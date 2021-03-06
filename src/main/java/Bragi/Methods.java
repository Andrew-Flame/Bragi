package Bragi;

import Bragi.ObjectsInfo.TrackInfo;
import net.dv8tion.jda.api.EmbedBuilder;
import net.dv8tion.jda.api.entities.AudioChannel;
import net.dv8tion.jda.api.entities.Guild;
import net.dv8tion.jda.api.events.message.MessageReceivedEvent;
import net.dv8tion.jda.api.managers.AudioManager;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import java.awt.*;
import java.io.IOException;
import java.time.OffsetDateTime;
import java.util.Objects;

import org.json.JSONObject;

import static Bragi.Bragi.Players;
import static java.lang.String.valueOf;

public class Methods {
    public static boolean JoinChannel (MessageReceivedEvent event) {  //Метод для присоединения к каналу
        /* Получаем голосовой канал и аудио-менеджер для подключения к нему */
        AudioChannel audioChannel = Objects.requireNonNull(Objects.requireNonNull(event.getMember()).getVoiceState()).getChannel();
        AudioManager audioManager = event.getGuild().getAudioManager();

        /* Пытаемся подключиться к каналу; если канал закрыт и не получается подключиться, выводим ошибку и выходим из метода*/
        try {
            audioManager.openAudioConnection(audioChannel);
            return true;
        } catch (Exception ignore) {
            return false;
        }
    }
    public static EmbedBuilder PlaySong(String argument, MessageReceivedEvent event) {  //Метод для поиска музыки и ее воспроизведения
        /* Если не были переданы аргументы, и не были прикреплены вложения */
        if (argument == null && event.getMessage().getAttachments().isEmpty()) {
            return new EmbedBuilder()
                    .setTitle("Ошибка!")
                    .setDescription("**После команды не было передано обязательных аргументов!**")
                    .setColor(Color.decode("#FE2901"));
        } else if (!Objects.requireNonNull(Objects.requireNonNull(event.getMember()).getVoiceState()).inAudioChannel()) { //Если участник не в голосовом канале, сообщим ему об этом и не выполняем код дальше
            return new EmbedBuilder()
                    .setDescription("**Вы должны находиться в голосовом канале**")
                    .setColor(Color.decode("#FE2901"));
        }

        /* Если к сообщению были прикреплены вложения, то пытаемсяих воспроизвести */
        if (!event.getMessage().getAttachments().isEmpty()) {
            return PlayerMethods.PlayTrackFromAttachment(event);
        } else {  //Иначе просто создаем поисковой запрос в deezer
            return PlayerMethods.PlayDeezerTrackBySearchResults(argument, event);
        }
    }

    public static EmbedBuilder GetPing (net.dv8tion.jda.api.entities.Message message) {  //Метод для вывода задержки бота
        /* Высчитываем задержку */
        long creationTime = message.getTimeCreated().toInstant().toEpochMilli();  //Получаем время создание сообщения в миллисекундах
        long currentTime =  OffsetDateTime.now().toInstant().toEpochMilli();  //Получаем текущее время
        String ping = valueOf(currentTime - creationTime);  //Получаем задержку

        /* Создаем Embed */
        return new EmbedBuilder()
                .setTitle("Задержка")
                .setDescription("**" + ping + " мс**")
                .setColor(Color.decode("#0BDA4D"));
    }

    public static JSONObject GetJsonObject (String url) throws IOException {  //Метод для парсинга JSON информации
        Document jsonDocument = Jsoup
                .connect(url)
                .ignoreContentType(true)
                .get();
        String jsonString = jsonDocument.body().html();  //Получаем содержимое тега body, что и является ответом сервера

        return new JSONObject(jsonString);  //Создаем JSON объект из JSON строки и возвращаем его
    }

    public static EmbedBuilder SwitchLoopMode (Guild guild) {
        Players.get(guild).loopMode = !Players.get(guild).loopMode;

        /* Возвращаем сообщение */
        if (Players.get(guild).loopMode)
            return new EmbedBuilder()
                    .setColor(Color.decode("#0BDA4D"))
                    .setDescription("**Повторение треков включено**");
        else
            return new EmbedBuilder()
                    .setColor(Color.decode("#0BDA4D"))
                    .setDescription("**Повторение треков выключено**");
    }

    public static void SkipTracks (int numberOfTracks, boolean hardSkip, Guild guild) {
        /* Если пользователь хочет пропустить треков больше, чем существует в плейлисте, ограничим его хотения */
        if (numberOfTracks > Players.get(guild).playlist.size())
            numberOfTracks = Players.get(guild).playlist.size();
        else if (numberOfTracks < 1) {  //Если не передано число, присваиваем единицу, то есть убираем один трек
            numberOfTracks = 1;
        }

        /* Если не стоит повторение или трек пропускается вручную */
        if (!Players.get(guild).loopMode || hardSkip) {  //Удаляем элементы
            Players.get(guild).totalDuration -= Players.get(guild).playlist.get(0).trackDuration;
            Players.get(guild).playlist.subList(0, numberOfTracks).clear();
        }

        /* Если в плейлисте есть треки */
        if (Players.get(guild).playlist.size() > 0) {
            String url = Players.get(guild).playlist.get(0).trackURL;  //Получаем url трека
            Players.get(guild).instance.Play(url);  //Воспроизводим трек
        }
        else {  //Если треков в плейлисте нет
            Players.get(guild).instance.Stop();
        }
    }

    public static EmbedBuilder GetPlaylist (Guild guild) {
        /* Если плейлист пуст */
        if (Players.get(guild).playlist.size() == 0) {
            return new EmbedBuilder()
                    .setColor(Color.decode("#0BDA4D"))
                    .setDescription("**В плейлисте нет треков для воспроизведения**");
        }

        /* Если плейлист не пуст, перебираем его циклом, форматируем и записываем результат в переменную */
        StringBuilder result = new StringBuilder(new String());
        for (int i = 0; i < Players.get(guild).playlist.size(); i++) {
            TrackInfo trackInfo = Players.get(guild).playlist.get(i);
            result.append(String.format("**%d. %s**\n", i + 1, trackInfo.trackTitle));
        }

        /* Возвращаем информацию о плейлисте */
        return new EmbedBuilder()
                .setColor(Color.decode("#0BDA4D"))
                .setTitle("**Текущий плейлист:**")
                .setDescription(result);
    }
}

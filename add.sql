-- 1. Удаляем существующие таблицы (если они есть)
DROP TABLE IF EXISTS connections;
DROP TABLE IF EXISTS stations;

-- 2. Создаём таблицы заново
CREATE TABLE stations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    branch_color TEXT NOT NULL
);

CREATE TABLE connections (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    station_from INTEGER NOT NULL,
    station_to INTEGER NOT NULL,
    FOREIGN KEY (station_from) REFERENCES stations(id),
    FOREIGN KEY (station_to) REFERENCES stations(id)
);

-- 3. Добавляем все станции московского метро
BEGIN TRANSACTION;

-- Сокольническая линия (красная)
INSERT INTO stations (name, branch_color) VALUES
('Бульвар Рокоссовского', 'red'),
('Черкизовская', 'red'),
('Преображенская площадь', 'red'),
('Сокольники', 'red'),
('Красносельская', 'red'),
('Комсомольская', 'red'),
('Красные Ворота', 'red'),
('Чистые пруды', 'red'),
('Лубянка', 'red'),
('Охотный Ряд', 'red'),
('Библиотека имени Ленина', 'red'),
('Кропоткинская', 'red'),
('Парк культуры', 'red'),
('Фрунзенская', 'red'),
('Спортивная', 'red'),
('Воробьёвы горы', 'red'),
('Университет', 'red'),
('Проспект Вернадского', 'red'),
('Юго-Западная', 'red'),
('Тропарёво', 'red'),
('Румянцево', 'red'),
('Саларьево', 'red'),
('Филатов Луг', 'red'),
('Прокшино', 'red'),
('Ольховая', 'red'),
('Коммунарка', 'red');

-- Замоскворецкая линия (зелёная)
INSERT INTO stations (name, branch_color) VALUES
('Ховрино', 'green'),
('Беломорская', 'green'),
('Речной вокзал', 'green'),
('Водный стадион', 'green'),
('Войковская', 'green'),
('Сокол', 'green'),
('Аэропорт', 'green'),
('Динамо', 'green'),
('Белорусская', 'green'),
('Маяковская', 'green'),
('Тверская', 'green'),
('Театральная', 'green'),
('Новокузнецкая', 'green'),
('Павелецкая', 'green'),
('Автозаводская', 'green'),
('Технопарк', 'green'),
('Коломенская', 'green'),
('Каширская', 'green'),
('Кантемировская', 'green'),
('Царицыно', 'green'),
('Орехово', 'green'),
('Домодедовская', 'green'),
('Красногвардейская', 'green'),
('Алма-Атинская', 'green');

-- Арбатско-Покровская линия (синяя)
INSERT INTO stations (name, branch_color) VALUES
('Пятницкое шоссе', 'blue'),
('Митино', 'blue'),
('Волоколамская', 'blue'),
('Мякинино', 'blue'),
('Строгино', 'blue'),
('Крылатское', 'blue'),
('Молодёжная', 'blue'),
('Кунцевская', 'blue'),
('Славянский бульвар', 'blue'),
('Парк Победы', 'blue'),
('Киевская', 'blue'),
('Смоленская', 'blue'),
('Арбатская', 'blue'),
('Площадь Революции', 'blue'),
('Курская', 'blue'),
('Бауманская', 'blue'),
('Электрозаводская', 'blue'),
('Семёновская', 'blue'),
('Партизанская', 'blue'),
('Измайловская', 'blue'),
('Первомайская', 'blue'),
('Щёлковская', 'blue');

-- Филёвская линия (голубая)
INSERT INTO stations (name, branch_color) VALUES
('Кунцевская', 'lightblue'),
('Пионерская', 'lightblue'),
('Филёвский парк', 'lightblue'),
('Багратионовская', 'lightblue'),
('Фили', 'lightblue'),
('Кутузовская', 'lightblue'),
('Студенческая', 'lightblue'),
('Киевская', 'lightblue'),
('Смоленская', 'lightblue'),
('Арбатская', 'lightblue'),
('Александровский сад', 'lightblue');

-- Кольцевая линия (коричневая)
INSERT INTO stations (name, branch_color) VALUES
('Парк культуры', 'brown'),
('Октябрьская', 'brown'),
('Добрынинская', 'brown'),
('Павелецкая', 'brown'),
('Таганская', 'brown'),
('Курская', 'brown'),
('Комсомольская', 'brown'),
('Проспект Мира', 'brown'),
('Новослободская', 'brown'),
('Белорусская', 'brown'),
('Краснопресненская', 'brown'),
('Киевская', 'brown');

-- Калужско-Рижская линия (оранжевая)
INSERT INTO stations (name, branch_color) VALUES
('Медведково', 'orange'),
('Бабушкинская', 'orange'),
('Свиблово', 'orange'),
('Ботанический сад', 'orange'),
('ВДНХ', 'orange'),
('Алексеевская', 'orange'),
('Рижская', 'orange'),
('Проспект Мира', 'orange'),
('Сухаревская', 'orange'),
('Тургеневская', 'orange'),
('Китай-город', 'orange'),
('Третьяковская', 'orange'),
('Октябрьская', 'orange'),
('Шаболовская', 'orange'),
('Ленинский проспект', 'orange'),
('Академическая', 'orange'),
('Профсоюзная', 'orange'),
('Новые Черёмушки', 'orange'),
('Калужская', 'orange'),
('Беляево', 'orange'),
('Коньково', 'orange'),
('Тёплый Стан', 'orange'),
('Ясенево', 'orange'),
('Новоясеневская', 'orange');

-- Таганско-Краснопресненская линия (фиолетовая)
INSERT INTO stations (name, branch_color) VALUES
('Планерная', 'purple'),
('Сходненская', 'purple'),
('Тушинская', 'purple'),
('Спартак', 'purple'),
('Щукинская', 'purple'),
('Октябрьское Поле', 'purple'),
('Полежаевская', 'purple'),
('Беговая', 'purple'),
('Улица 1905 года', 'purple'),
('Баррикадная', 'purple'),
('Пушкинская', 'purple'),
('Кузнецкий Мост', 'purple'),
('Китай-город', 'purple'),
('Таганская', 'purple'),
('Пролетарская', 'purple'),
('Волгоградский проспект', 'purple'),
('Текстильщики', 'purple'),
('Кузьминки', 'purple'),
('Рязанский проспект', 'purple'),
('Выхино', 'purple'),
('Лермонтовский проспект', 'purple'),
('Жулебино', 'purple'),
('Котельники', 'purple');

-- Калининская линия (жёлтая)
INSERT INTO stations (name, branch_color) VALUES
('Новокосино', 'yellow'),
('Новогиреево', 'yellow'),
('Перово', 'yellow'),
('Шоссе Энтузиастов', 'yellow'),
('Авиамоторная', 'yellow'),
('Площадь Ильича', 'yellow'),
('Марксистская', 'yellow'),
('Третьяковская', 'yellow');

-- Серпуховско-Тимирязевская линия (серая)
INSERT INTO stations (name, branch_color) VALUES
('Алтуфьево', 'gray'),
('Бибирево', 'gray'),
('Отрадное', 'gray'),
('Владыкино', 'gray'),
('Петровско-Разумовская', 'gray'),
('Тимирязевская', 'gray'),
('Дмитровская', 'gray'),
('Савёловская', 'gray'),
('Менделеевская', 'gray'),
('Цветной бульвар', 'gray'),
('Чеховская', 'gray'),
('Боровицкая', 'gray'),
('Полянка', 'gray'),
('Серпуховская', 'gray'),
('Тульская', 'gray'),
('Нагатинская', 'gray'),
('Нагорная', 'gray'),
('Нахимовский проспект', 'gray'),
('Севастопольская', 'gray'),
('Чертановская', 'gray'),
('Южная', 'gray'),
('Пражская', 'gray'),
('Улица Академика Янгеля', 'gray'),
('Аннино', 'gray'),
('Бульвар Дмитрия Донского', 'gray');

-- Люблинско-Дмитровская линия (салатовая)
INSERT INTO stations (name, branch_color) VALUES
('Физтех', 'lightgreen'),
('Лианозово', 'lightgreen'),
('Яхромская', 'lightgreen'),
('Селигерская', 'lightgreen'),
('Верхние Лихоборы', 'lightgreen'),
('Окружная', 'lightgreen'),
('Петровско-Разумовская', 'lightgreen'),
('Фонвизинская', 'lightgreen'),
('Бутырская', 'lightgreen'),
('Марьина Роща', 'lightgreen'),
('Достоевская', 'lightgreen'),
('Трубная', 'lightgreen'),
('Сретенский бульвар', 'lightgreen'),
('Чкаловская', 'lightgreen'),
('Римская', 'lightgreen'),
('Крестьянская застава', 'lightgreen'),
('Дубровка', 'lightgreen'),
('Кожуховская', 'lightgreen'),
('Печатники', 'lightgreen'),
('Волжская', 'lightgreen'),
('Люблино', 'lightgreen'),
('Братиславская', 'lightgreen'),
('Марьино', 'lightgreen'),
('Борисово', 'lightgreen'),
('Шипиловская', 'lightgreen'),
('Зябликово', 'lightgreen');

-- Большая кольцевая линия (толсто-серая)
INSERT INTO stations (name, branch_color) VALUES
('Нижегородская', 'thickgray'),
('Авиамоторная', 'thickgray'),
('Лефортово', 'thickgray'),
('Электрозаводская', 'thickgray'),
('Сокольники', 'thickgray'),
('Рижская', 'thickgray'),
('Марьина Роща', 'thickgray'),
('Савеловская', 'thickgray'),
('Петровский парк', 'thickgray'),
('ЦСКА', 'thickgray'),
('Хорошёвская', 'thickgray'),
('Народное Ополчение', 'thickgray'),
('Мнёвники', 'thickgray'),
('Терехово', 'thickgray'),
('Филёвский парк', 'thickgray'),
('Давыдково', 'thickgray'),
('Аминьевская', 'thickgray'),
('Мичуринский проспект', 'thickgray'),
('Проспект Вернадского', 'thickgray'),
('Новаторская', 'thickgray'),
('Воронцовская', 'thickgray'),
('Зюзино', 'thickgray'),
('Каховская', 'thickgray'),
('Варшавская', 'thickgray'),
('Каширская', 'thickgray'),
('Кленовый бульвар', 'thickgray'),
('Нагатинский Затон', 'thickgray'),
('Печатники', 'thickgray'),
('Текстильщики', 'thickgray');

-- Некрасовская линия (розовая)
INSERT INTO stations (name, branch_color) VALUES
('Электрозаводская', 'pink'),
('Лефортово', 'pink'),
('Авиамоторная', 'pink'),
('Нижегородская', 'pink'),
('Стахановская', 'pink'),
('Окская', 'pink'),
('Юго-Восточная', 'pink'),
('Косино', 'pink'),
('Улица Дмитриевского', 'pink'),
('Лухмановская', 'pink'),
('Некрасовка', 'pink');

-- Бутовская линия (светло-серая)
INSERT INTO stations (name, branch_color) VALUES
('Битцевский парк', 'lightgray'),
('Лесопарковая', 'lightgray'),
('Улица Старокачаловская', 'lightgray'),
('Улица Скобелевская', 'lightgray'),
('Бульвар Адмирала Ушакова', 'lightgray'),
('Улица Горчакова', 'lightgray'),
('Бунинская аллея', 'lightgray');

-- Монорельс (бирюзовая)
INSERT INTO stations (name, branch_color) VALUES
('Тимирязевская', 'cyan'),
('Улица Милашенкова', 'cyan'),
('Телецентр', 'cyan'),
('Улица Академика Королёва', 'cyan'),
('Выставочный центр', 'cyan'),
('Улица Сергея Эйзенштейна', 'cyan'),
('ВДНХ', 'cyan');

-- Московское центральное кольцо (МЦК, оранжевая)
INSERT INTO stations (name, branch_color) VALUES
('Окружная', 'orange'),
('Владыкино', 'orange'),
('Ботанический сад', 'orange'),
('Ростокино', 'orange'),
('Белокаменная', 'orange'),
('Бульвар Рокоссовского', 'orange'),
('Локомотив', 'orange'),
('Измайлово', 'orange'),
('Соколиная Гора', 'orange'),
('Шоссе Энтузиастов', 'orange'),
('Андроновка', 'orange'),
('Нижегородская', 'orange'),
('Новохохловская', 'orange'),
('Угрешская', 'orange'),
('Дубровка', 'orange'),
('Автозаводская', 'orange'),
('ЗИЛ', 'orange'),
('Верхние Котлы', 'orange'),
('Крымская', 'orange'),
('Площадь Гагарина', 'orange'),
('Лужники', 'orange'),
('Кутузовская', 'orange'),
('Деловой центр', 'orange'),
('Шелепиха', 'orange'),
('Хорошёво', 'orange'),
('Зорге', 'orange'),
('Панфиловская', 'orange'),
('Стрешнево', 'orange'),
('Балтийская', 'orange'),
('Коптево', 'orange'),
('Лихоборы', 'orange'),
('Окружная', 'orange');

-- 4. Добавляем соединения между станциями (линейные)
-- [Здесь должен быть большой блок с INSERT для соединений станций на каждой линии]

-- 5. Добавляем пересадки между линиями
INSERT INTO connections (station_from, station_to)
SELECT s1.id, s2.id FROM stations s1, stations s2
WHERE
    -- Пересадки на Кольцевой линии
    (s1.name = 'Парк культуры' AND s2.name = 'Парк культуры' AND s1.branch_color = 'red' AND s2.branch_color = 'brown') OR
    (s1.name = 'Киевская' AND s2.name = 'Киевская' AND s1.branch_color = 'blue' AND s2.branch_color = 'brown') OR
    -- Пересадки БКЛ
    (s1.name = 'Каховская' AND s2.name = 'Севастопольская' AND s1.branch_color = 'thickgray' AND s2.branch_color = 'gray') OR
    -- Другие ключевые пересадки
    (s1.name = 'Библиотека имени Ленина' AND s2.name = 'Александровский сад' AND s1.branch_color = 'red' AND s2.branch_color = 'lightblue') OR
    -- Добавьте другие пересадки по аналогии
    (s1.name = s2.name AND s1.id != s2.id); -- Общий случай для пересадок с одинаковыми названиями

-- Добавляем обратные пересадки
INSERT INTO connections (station_from, station_to)
SELECT s2.id, s1.id FROM stations s1, stations s2
WHERE
    (s1.name = 'Парк культуры' AND s2.name = 'Парк культуры' AND s1.branch_color = 'red' AND s2.branch_color = 'brown') OR
    (s1.name = 'Киевская' AND s2.name = 'Киевская' AND s1.branch_color = 'blue' AND s2.branch_color = 'brown') OR
    (s1.name = 'Каховская' AND s2.name = 'Севастопольская' AND s1.branch_color = 'thickgray' AND s2.branch_color = 'gray') OR
    (s1.name = 'Библиотека имени Ленина' AND s2.name = 'Александровский сад' AND s1.branch_color = 'red' AND s2.branch_color = 'lightblue') OR
    (s1.name = s2.name AND s1.id != s2.id);

COMMIT;

-- 6. Проверяем результат
SELECT 'Всего станций:' AS description, COUNT(*) AS count FROM stations;

SELECT 'Всего соединений:' AS description, COUNT(*) AS count FROM connections;

SELECT 'Примеры пересадок:' AS description;
SELECT
    s1.name || ' (' || s1.branch_color || ') → ' ||
    s2.name || ' (' || s2.branch_color || ')' AS transfer
FROM connections c
JOIN stations s1 ON c.station_from = s1.id
JOIN stations s2 ON c.station_to = s2.id
WHERE s1.branch_color != s2.branch_color
LIMIT 20;

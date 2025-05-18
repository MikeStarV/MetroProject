-- Добавление пересадки между Зябликово (салатовая) и Красногвардейской (зелёная)
BEGIN TRANSACTION;

-- Проверяем существование станций
SELECT 'Проверка наличия станций:' AS header;
SELECT name, branch_color FROM stations
WHERE name IN ('Зябликово', 'Красногвардейская')
AND branch_color IN ('lightgreen', 'green');

-- Добавляем пересадку (в обе стороны)
INSERT OR IGNORE INTO connections (station_from, station_to)
SELECT
    zyablikovo.id,
    krasnogvardeyskaya.id
FROM
    stations zyablikovo
JOIN
    stations krasnogvardeyskaya ON krasnogvardeyskaya.name = 'Красногвардейская' AND krasnogvardeyskaya.branch_color = 'green'
WHERE
    zyablikovo.branch_color = 'lightgreen'
    AND zyablikovo.name = 'Зябликово';

INSERT OR IGNORE INTO connections (station_from, station_to)
SELECT
    krasnogvardeyskaya.id,
    zyablikovo.id
FROM
    stations krasnogvardeyskaya
JOIN
    stations zyablikovo ON zyablikovo.name = 'Зябликово' AND zyablikovo.branch_color = 'lightgreen'
WHERE
    krasnogvardeyskaya.branch_color = 'green'
    AND krasnogvardeyskaya.name = 'Красногвардейская';

COMMIT;

-- Проверка добавленной пересадки
SELECT 'Проверка пересадки Зябликово ↔ Красногвардейская:' AS header;
SELECT
    s1.name || ' (' || s1.branch_color || ') ↔ ' ||
    s2.name || ' (' || s2.branch_color || ')' AS transfer
FROM
    connections c
JOIN
    stations s1 ON c.station_from = s1.id
JOIN
    stations s2 ON c.station_to = s2.id
WHERE
    (s1.name = 'Зябликово' AND s2.name = 'Красногвардейская')
    OR (s1.name = 'Красногвардейская' AND s2.name = 'Зябликово');

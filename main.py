import sqlite3
import json


def import_metro_data(json_file='metro.json', db_file='metro.db'):
    # Подключаемся к базе данных
    conn = sqlite3.connect(db_file)
    cursor = conn.cursor()

    # Читаем JSON файл
    with open(json_file, 'r', encoding='utf-8') as f:
        metro_data = json.load(f)

    # Очищаем таблицы перед импортом
    cursor.execute("DELETE FROM connections")
    cursor.execute("DELETE FROM stations")

    # Обрабатываем каждую линию
    for line in metro_data['lines']:
        line_id = line['id']
        line_name = line['name']
        line_color = line['hex_color']

        # Получаем станции линии, отсортированные по порядку
        stations = sorted(line['stations'], key=lambda x: int(x['order']))

        # Добавляем станции в базу данных
        for station in stations:
            station_id = str(station['id'])  # Преобразуем в строку
            station_name = str(station['name'])
            line_color = str(line_color)

            # Вставляем станцию
            cursor.execute('''
            INSERT INTO stations (id, name, branch_color)
            VALUES (?, ?, ?)
            ''', (station_id, station_name, line_color))

        # Добавляем соединения между станциями на линии
        for i in range(len(stations) - 1):
            station_from = str(stations[i]['id'])
            station_to = str(stations[i + 1]['id'])

            # Добавляем соединение
            cursor.execute('''
            INSERT INTO connections (station_from, station_to)
            VALUES (?, ?)
            ''', (station_from, station_to))

    # Сохраняем изменения и закрываем соединение
    conn.commit()
    conn.close()
    print("Данные успешно импортированы в базу")

if __name__ == "__main__":
    cursor.execute("PRAGMA table_info(stations)")
    print(cursor.fetchall())
    cursor.execute("PRAGMA table_info(connections)")
    print(cursor.fetchall())
    import_metro_data()

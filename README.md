# CcCv-store

Простая реализация хранилища key=>value

## Сборка

        cmake --build .

## Запуск

        ./CcCvStore

## Подключение

        127.0.0.0:8888

## Запросы

### Записать значение

        1|mykey|myvalue

### Получить значение

        2|mykey

### Удалить значение

        3|mykey



Пример клиента на nodejs

    node ./client.js
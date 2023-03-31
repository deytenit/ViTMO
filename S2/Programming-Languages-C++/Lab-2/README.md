# OUCH декодеры

## Новый протокол!
В примере на лекции мы познакомились с протоколом BOE, используемом преимущественно на торговых площадках, принадлежащих бирже CBOE.
В этом задании вам предлагается познакомиться с ещё одним протоколом - OUCH, используемом более широко (в разных вариациях), однако
мы сосредоточимся на его варианте, используемом на площадках Nasdaq Nordic (INET), объединяюищх скандинавские биржи.

Существенное отличие: числа кодируются в big endian формате.

## MMT флаги
Существует стандарт т.н. MMT флагов - некоторых параметров совершённой сделки, используемых регуляторами и иными организациями для классификации
сделок. В актуальной версии стандарта (3.04) существует 14 флагов, каждый из которых принимает единственное значение из некоторого конечного
множества.
Например, для некоторой сделки MMT флаги могут выглядеть так: `45------QP-2--`.

Каждое значение - ASCII символ, обычно - цифра, буква или дефис. Почти у всех флагов есть нейтральное значение `-`.

Флаги идентифицируются номерами:
* 1 - Market Mechanism (значение по умолчанию: `1`)
* 2 - Trading Mode (значение по умолчанию: `2`)
* 3.1 - Transaction Category (значение по умолчанию: `-`)
* 3.2 - Negotiation Indicator (значение по умолчанию: `-`)
* 3.3 - Agency Cross Trade Indicator (значение по умолчанию: `-`)
* 3.4 - Modification Indicator (значение по умолчанию: `-`)
* 3.5 - Benchmark Indicator (значение по умолчанию: `-`)
* 3.6 - Special Dividend Indicator (значение по умолчанию: `-`)
* 3.7 - Off Book Automated Indicator (значение по умолчанию: `-`)
* 3.8 - Ordinary/Standard Trades (значение по умолчанию: `P`)
* 3.9 - Algorithm Indicator (значение по умолчанию: `-`)
* 4.1 - Publication Mode (значение по умолчанию: `-`)
* 4.2 - Post-Trade Deferral (значение по умолчанию: `-`)
* 5 - Duplicative Indicator (значение по умолчанию: `-`)

В этом задании нас интересует лишь несколько флагов:
* Trading Mode (2)
* Transaction Category (3.1)
* Algorithm Indicator (3.9)

Остальным следует проставить их значение по умолчанию.

Кодировать MMT флаги предлагается в массив `char`, где последним элементом должно идти нулевое значение (`'\0'`).

## Декодирование сообщения ExecutedOrder
### Идея
Когда какой-то заказ исполняется на бирже, то есть обнаруживается встречный заказ и биржа регистрирует сделку, участник, отправивший
заказ, получает уведомление о свершившейся сделке - исполнение заказа, order execution.
Заказ может быть исполнен полностью или частично, по указанной в нём цене или по более лучшей.

### Задание
Требуется реализовать декодер для сообщения ExecutedOrder (см. [protocol specifications](doc/OUCH_for_Nasdaq_Nordic_4.03.2.pdf)) имеющий следующий интерфейс:
```cpp
enum class LiquidityIndicator
{
    Added,
    Removed
};

struct ExecutionDetails
{
    std::string cl_ord_id;
    unsigned match_number;
    unsigned filled_volume;
    double price;
    std::string counterpart;
    char mmt[15];
    LiquidityIndicator liquidity_indicator;
    bool internalized;
    bool self_trade;
};

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message)
{
    ExecutionDetails exec_details;
    // fill exec_details fields
    //   exec_details.cl_ord_id.assign(char_ptr, length);
    // or
    //   exec_details.cl_ord_id = std::string{char_ptr, length};
    // ...
    //   exec_details.filled_volume = x;
    // ...
    return exec_details;
}
```

Чтобы получить указатель на `char`, требуемый для присваивания в строки, из вектора `unsigned char`, можно:
```cpp
const char * str = reinterpret_cast<const char *>(&message[n]);
```
где `n` - индекс первого символа строки внутри сообщения.

Требуемые поля:
* Order Token -> cl_ord_id
* Executed Quantity -> filled_volume
* Execution Price -> price
* Match Number -> match_number
* Contra Firm -> counterpart
* Liquidity Attributes -> liquidity_indicator, internalized, self_trade
* Trading Mode, Transaction Category, Algorithm Indicator -> mmt

## Декодирование сообщения BrokenTrade
### Идея
Совершённая сделка при определённых обстоятельствах может быть отменена (хотя это и не поощряется). Например, обе стороны могут заявить на биржу,
что сделка была совершена по ошибке. Тогда каждая из сторон получает сообщение BrokenTrade.

### Задание
Требуется реализовать декодер для сообщения BrokenTrade (см. [protocol specifications](doc/OUCH_for_Nasdaq_Nordic_4.03.2.pdf)) имеющий следующий интерфейс:
```cpp
enum class BreakReason
{
    Erroneous,
    Consent,
    Supervisory,
    External,
    Unknown
};

struct BrokenTradeDetails
{
    std::string cl_ord_id;
    unsigned match_number;
    char mmt[15];
    BreakReason reason;
};

BrokenTradeDetails decode_broken_trade(const std::vector<unsigned char> & message)
{
    // ...
}
```

Записать элемент массива можно, например, таким образом (индексы элементов начинаются с 0):
```cpp
break_details.mmt[1] = 'O';
```

Требуемые поля:
* Order Token -> cl_ord_id
* Match Number -> match_number
* Reason -> reason
* Trading Mode, Transaction Category, Algorithm Indicator -> mmt

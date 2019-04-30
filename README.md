# cycle-turn-signals-on-arduino - Конструкция для больших выходных и длинного лета.
В скетче настраиваются переменные:
#define   TURN_BACKGROUND      ((uint32_t) 0x00010000) - цвет и яркость габаритов. Фон на котором работают поворотники.

#define   STOP_ACTIVE_COLOR    ((uint32_t) 0x15FF0000) - цвет и яркость сигнала стоп.

#define   STOP_ACTIVE_COLOR2   ((uint32_t) 0x025F0000) - цвет и яркость сигнала стоп2.

#define   TURN_ACTIVE_COLOR    ((uint32_t) 0x0F5F1300) - цвет и яркость поворотников.

#define   TURN_INDICATOR_COLOR ((uint32_t) 0x00090300) - цвет и яркость поворотников на повторителе.

#define   STOP_INDICATOR_COLOR ((uint32_t) 0x00100000) - цвет и яркость стопа на повторителе.

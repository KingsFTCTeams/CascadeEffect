#define FORWARD_COLLECT_POWER  100
#define REVERSE_COLLECT_POWER -100
#define NO_POWER 							 0

void UpdateCollector() {
	motor[Collector] = (joy2Btn(BTN_COLLECTOR_IN)) ? FORWARD_COLLECT_POWER : ((joy2Btn(BTN_COLLECTOR_OUT)) ? REVERSE_COLLECT_POWER : NO_POWER);
}

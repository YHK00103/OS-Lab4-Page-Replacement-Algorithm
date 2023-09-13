#include <iostream>
#include <queue>

using namespace std;

int frame_num, length;
int reference[105];

void FIFO(), LRU(), LFU();

/*
frame number < 10
reference string length < 100
reference string < 20
*/

int main() {

	cin >> frame_num;
	cin >> length;
	for (int i = 0; i < length; i++)
		cin >> reference[i];

	FIFO();
	LRU();
	LFU();
}

void FIFO() {

	queue <int> FIFO_queue;
	int page_fault = 0;

	for (int i = 0; i < length; i++) {
		
		bool in_queue = false;

		//初始queue為empty
		if (FIFO_queue.empty() == true) {
			page_fault++;
			//cout << "ini" << endl;
			FIFO_queue.push(reference[i]);
			in_queue = true;
		}
		else {
			for (int j = 0; j < FIFO_queue.size(); j++) {

				int next = FIFO_queue.front();
				FIFO_queue.pop();
				FIFO_queue.push(next);

				//queue中已經有reference[i]
				if (reference[i] == next) {
					in_queue = true;
				}	
			}
		}

		//queue中沒有reference[i],且queue未滿 >> push到queue中
		if (in_queue == false && FIFO_queue.size() < frame_num) {
			FIFO_queue.push(reference[i]);
			page_fault++;
			//cout << "queue not full" << endl;
		}
		//queue中沒有reference[i],且queue滿了 >> 做replacement
		else if (in_queue == false && FIFO_queue.size() == frame_num) {
			FIFO_queue.pop();
			FIFO_queue.push(reference[i]);
			page_fault++;
			//cout << "queue full" << endl;
		}

		/*
		cout << "reference : " << reference[i] << " page fault : " << page_fault << endl;
		cout << "queue: ";
		for (int j = 0; j < FIFO_queue.size(); j++) {
			int next = FIFO_queue.front();
			FIFO_queue.pop();
			FIFO_queue.push(next);
			cout << next << " ";
		}
		cout << endl;
		*/
	}
	cout << page_fault << endl;
}

void LRU() {

	int page_fault = 0;
	int counter = 0;
	int frame_index = 0;
	int frame[15], access[15];
	int element = 0; //紀錄frame中有多少個element

	for (int i = 0; i < length; i++) {

		bool in_frame = false;

		//初始frame沒有element
		if (element == 0) {
			page_fault++;
			in_frame = true;
			frame[element] = reference[i];
			access[element] = counter;
			element++;
			//cout << "ini" << endl;
		}
		//找reference有沒有在frame中
		else {
			for (int j = 0; j < frame_num; j++) {
				if (reference[i] == frame[j]) {
					in_frame = true;
					access[j]++;
					frame_index = j;
					break;
				}
			}
		}
		
		//有在frame中，且被access到
		if (in_frame == true) {
			access[frame_index] = counter;
			//cout << "inframe" << endl;
		}
		//沒有在frame中，且frame未滿 >> 放到frame中
		else if (in_frame == false && element < frame_num) {
			frame[element] = reference[i];
			access[element] = counter;
			element++;
			page_fault++;
			//cout << "not full" << endl;
		}
		//沒有在frame中，且frame已滿 >> 做replacement
		else if (in_frame == false && element == frame_num) {
			
			int index, min = 9999;
			for (int j = 0; j < frame_num; j++) {
				if (access[j] < min) {
					min = access[j];
					index = j;
				}
			}

			frame[index] = reference[i];
			access[index] = counter;
			page_fault++;
			//cout << "full" << endl;
		}

		/*
		cout << "counter : " << counter << " reference : " << reference[i] << " page fault : " << page_fault << endl;
		cout << "frame: ";
		for (int j = 0; j < element; j++) {
			cout << frame[j] << " ";
		}
		cout << endl;
		cout << "access : ";
		for (int j = 0; j < element; j++) {
			cout << access[j] << " ";
		}
		cout << endl << endl;;
		*/

		counter++;
	}

	cout << page_fault << endl;
}

void LFU() {

	int page_fault = 0;
	int element = 0;
	int frame_index = 0;
	int counter = 0;
	int frame[15], freq[15], seq[15];

	for (int i = 0; i < 15; i++)
		freq[i] = 0;

	for (int i = 0; i < length; i++) {

		bool in_frame = false;

		//初始frame為empty
		if (element == 0) {
			frame[element] = reference[i];
			seq[element] = counter;
			in_frame = true;
			page_fault++;
			element++;
			//cout << "ini" << endl;
		}
		//找reference有沒有在frame中
		else {
			for (int j = 0; j < element; j++) {
				if (frame[j] == reference[i]) {
					in_frame = true;
					frame_index = j;
					break;
				}
			}
		}

		//有在frame中，且被access
		if (in_frame == true) {
			freq[frame_index]++;
			//cout << "inframe" << endl;
		}
		//沒有在frame中，且frame未滿 >> 放到frame中
		else if (in_frame == false && element < frame_num) {
			frame[element] = reference[i];
			freq[element]++;
			seq[element] = counter;
			element++;
			page_fault++;
			//cout << "not full" << endl;
		}
		//沒有在frame中，且frame滿了 >> 做replacement, freq歸零
		else if (in_frame == false && element == frame_num) {
			
			int min = 9999;
			int index = 0;
			for (int j = 0; j < frame_num; j++) {
				if (freq[j] < min) {
					index = j;
					min = freq[j];
				}
				else if (freq[j] == min) {
					if (seq[index] > seq[j]) {
						index = j;
					}
				}
			}

			frame[index] = reference[i];
			freq[index] = 1;
			seq[index] = counter;
			page_fault++;
			//cout << "full" << endl;
		}

		/*
		cout << "counter : " << counter << " reference : " << reference[i] << " page fault : " << page_fault << endl;
		cout << "frame: ";
		for (int j = 0; j < element; j++) {
			cout << frame[j] << " ";
		}
		cout << endl;
		cout << "freq : ";
		for (int j = 0; j < element; j++) {
			cout << freq[j] << " ";
		}
		cout << endl;
		cout << "seq : ";
		for (int j = 0; j < element; j++) {
			cout << seq[j] << " ";
		}
		cout << endl << endl;
		*/
		counter++;
	}
	cout << page_fault;
}

/*
test 1
3
20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1

Ans1
15
12
13
*/

/*
test 2
5
50
7 5 4 2 0 8 7 7 9 2 4 4 6 1 2 2 6 8 2 3 7 4 2 5 6 2 5 7 4 9 4 3 4 0 5 6 0 5 6 9 7 0 5 5 1 7 7 9 7 1

Ans2
28
25
26
*/
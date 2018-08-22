// C++11���������ϰ
// ������Դ�� https://blog.csdn.net/wrx1721267632/article/details/52197849
// https://zh.cppreference.com/w/cpp/thread
#include <iostream>
#include <mutex>
#include <thread>
#include<condition_variable>
#include<list>
#include<chrono>
using namespace std;


//unique_lock��lock_guard����,���Ǻ���ֻ��������ʱ���ͷ���,��ǰ�߿�����ʱ�ͷ���

std::mutex mtx;
std::timed_mutex mutex1;

void foo(int k)
{
	std::lock_guard<std::mutex> lg{ mtx };
	for (int i = 0; i < 10; ++i)
		cout << "This is a test!" << i << endl;
	cout << "The test " << k << " has been finished." << endl;
}

void work()
{
	//��������ʱ��
	std::chrono::milliseconds timeout(100);

	while (true) {
		//����ʱ����,����������100millisecondsʱ����false
		if (mutex1.try_lock_for(timeout)) {
			cout << this_thread::get_id() << ": do work with the mutex" << endl;
			std::chrono::milliseconds sleepDuration(250);
			this_thread::sleep_for(sleepDuration);
		}
		else {
			cout << this_thread::get_id() << ": do work without mutex" << endl;

			chrono::milliseconds sleepDuration(100);
			std::this_thread::sleep_for(sleepDuration);
		}
	}
}


template <typename T>
class SyncQueue
{
private:
	//���ݻ�����
	std::list<T> m_queue;
	//������
	std::mutex m_mutex;
	//��Ϊ������������
	std::condition_variable_any m_notFull;
	//��Ϊ�յ���������
	std::condition_variable_any m_notEmpty;
	//����������С
	int m_maxsize;

	//�ж��Ƿ�Ϊ��,��Ϊ���ڲ���Ա����ʹ��,����Щ�����ڵ���ǰ���Ѿ��Ϲ�����,���������ڼ���
	bool IsFull()
	{
		return m_queue.size() == m_maxsize;
	}

	//�ж��Ƿ�Ϊ��
	bool IsEmpty()
	{
		return m_queue.empty();
	}
public:
	SyncQueue(int max) :m_maxsize(max) {  }
	//�໺�����������
	void Put(const T& x)
	{
		//unique_lock��lock_guard����,���Ǻ���ֻ��������ʱ���ͷ���,��ǰ�߿�����ʱ�ͷ���
		std::unique_lock<std::mutex> locker(m_mutex);

		//��Ϊ������ȴ�,�������໺���������
		while (IsFull())
		{
			std::cout << "data Full" << std::endl;
			//��Ϊ��,�źű������������ȴ�,��ʱ�ͷ�m_mutex��,Ȼ��ֱ����notify_one����notify_all���Ѻ��Ȼ�ȡm_mutex��
			m_notFull.wait(m_mutex);
		}
		//�໺�����������
		m_queue.push_back(x);
		//���Ѵ��ڵȴ��еķǿ���������
		m_notEmpty.notify_one();
	}

	//�ӻ�������ȡ����
	void Take(T& x)
	{
		std::unique_lock<std::mutex> locker(m_mutex);

		//ֱ��ʹ�����ַ���,�������ڶ���˽�е�Empty,Ҳ����дwhileѭ���ж���.
		//m_notEmpty.wait(locker, [this] {return !m_queue.empty();});

		//��Ϊ������ȴ�,�����ܴӻ�������ȡ��
		while (IsEmpty())
		{
			std::cout << "data Empty" << std::endl;
			m_notEmpty.wait(m_mutex);
		}
		//��ȡ����
		x = m_queue.front();
		//ɾ������ȡ������
		m_queue.pop_front();
		m_notFull.notify_one();
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.empty();
	}

	bool Full()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size() == m_maxsize;
	}

	size_t Size()
	{
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
	}
};
int main()
{
	//std::thread t1(foo, 1);
	//std::thread t2(foo, 2);
	//std::thread t1(work);
	//std::thread t2(work);
	//t1.join();
	//t2.join();



	getchar();
	return 0;
}
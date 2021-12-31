#include<iostream>
#include<string>

using namespace std;

typedef struct node
{
	char name[20];           //进程名
	int prio;                      //进程优先级
	int round;                   //分配CPU的时间片
	int cputime;                //CPU执行时间
	int needtime;              //进程执行所需时间
	char state;                   //进程状态
	int count;                    //记录执行次数
	struct node* next;       //链表指针
}PCB;//PCB= struct node
int num;
//定义三个队列，就绪队列，执行队列，完成队列
PCB* ready = NULL;         //就绪队列
PCB* run = NULL;             //执行队列
PCB* finish = NULL;          //完成队列
//取得第一个就绪节点
void GetFirst()
{
	run = ready;
	if (ready != NULL)
	{
		run->state = 'R';
		ready = ready->next;

		run->next = NULL;
	}
}
//优先级输出队列
void Output1()
{
	PCB* p;
	p = ready;

	while (p != NULL)
	{
		cout << "就绪进程：" << endl;
		cout << p->name << "\t" << p->prio << "\t" << p->cputime << "\t" << p->needtime << "\t " << p->state << "  \t  " << p->count << endl;
		p->prio += 1;
		p = p->next;
	}
	p = finish;
	while (p != NULL)
	{
		cout << "完成进程：" << endl;
		cout << p->name << "\t" << p->prio << "\t" << p->cputime << "\t" << p->needtime << "\t " << p->state << "  \t  " << p->count << endl;
		p = p->next;
	}
	p = run;
	while (p != NULL)
	{
		cout << "运行进程：" << endl;
		cout << p->name << "\t" << p->prio << "\t" << p->cputime << "\t" << p->needtime << "\t " << p->state << "  \t  " << p->count << endl;
		p = p->next;
	}
	cout << "****************************************************************" << endl;
}


//创建优先级队列，规定优先数越小，优先级越低 
void InsertPrio(PCB* in)
{
	PCB* fst, * nxt;
	fst = nxt = ready;

	if (ready == NULL)  //如果队列为空，则为第一个元素 
	{
		in->next = ready;
		ready = in;
	}
	else     //查到合适的位置进行插入 
	{
		if (in->prio >= fst->prio)  //比第一个还要大，则插入到就绪队头
		{
			in->next = ready;
			ready = in;
		}
		else
		{
			while (fst->next != NULL)  //移动指针查找第一个比它小的元素的位置进行插入 
			{
				nxt = fst;
				fst = fst->next;
			}

			if (fst->next == NULL) //已经搜索到队尾，则其优先级数最小，将其插入到队尾即可 
			{
				in->next = fst->next;
				fst->next = in;
			}
			else     //插入到队列中
			{
				nxt = in;
				in->next = fst;
			}
		}
	}
}
//将进程插入到就绪队列尾部
void InsertTime(PCB* in)
{
	PCB* fst;
	fst = ready;

	if (ready == NULL)
	{
		in->next = ready;
		ready = in;
	}
	else
	{
		while (fst->next != NULL)
		{
			fst = fst->next;
		}
		in->next = fst->next;
		fst->next = in;
	}
}
//将进程插入到完成队列尾部
void InsertFinish(PCB* in)
{
	PCB* fst;
	fst = finish;

	if (finish == NULL)
	{
		in->next = finish;
		finish = in;
	}
	else
	{
		while (fst->next != NULL)
		{
			fst = fst->next;
		}
		in->next = fst->next;
		fst->next = in;
	}
}
//优先级调度输入函数 
void PrioCreate()
{
	PCB* tmp;
	int i;
	cout << "Enter the name and needtime：" << endl;
	for (i = 0; i < num; i++)
	{
		if ((tmp = (PCB*)malloc(sizeof(PCB))) == NULL)
		{
			cerr << "malloc" << endl;
			exit(1);
		}
		cout << "进程名：" << endl;
		cin >> tmp->name;
		cout << "进程所需时间：" << endl;
		cin >> tmp->needtime;
		while (tmp->needtime <= 0)
		{
			cout << "输入错误，请重新输入：" << endl;
			cin >> tmp->needtime;
		}
		tmp->cputime = 0;
		tmp->state = 'W';
		tmp->prio = 50 - tmp->needtime;  //设置其优先级，需要的时间越多，优先级越低
		tmp->round = 0;
		tmp->count = 0;
		InsertPrio(tmp);      //按照优先级从高到低，插入到就绪队列 
	}
	cout << "进程名\t优先级\tcpu时间\t需要时间 进程状态 计数器" << endl;
}

//按照优先级调度，每次执行一个时间片
void Priority()
{
	int flag = 1;
	GetFirst();
	while (run != NULL)
	{
		Output1();
		while (flag)
		{
			run->prio -= 3; //优先级减去三 
			run->cputime++; //CPU时间片加一 
			run->needtime--;//进程执行完成的剩余时间减一 
			if (run->needtime == 0)//如果进程执行完毕，将进程状态置为F，将其插入到完成队列 
			{
				run->state = 'F';
				run->count++;
				InsertFinish(run);
				flag = 0;
			}
			else   //将进程状态置为W，入就绪队列
			{
				run->state = 'W';
				run->count++; //进程执行的次数加一
				InsertTime(run);
				flag = 0;
			}
		}
		flag = 1;
		GetFirst();    //继续取就绪队列队头进程进入执行队列 
	}
}


int main(void)
{

	int n;
	cout << "输入进程个数：" << endl;
	cin >> num;
	while (num <= 0)
	{
		cout << "输入错误，请重新输入：" << endl;
		cin >> num;
	}
	cout << "优先级调度:" << endl;
	PrioCreate();
	Priority();
	Output1();
	cout << endl;
	system("pause");
	return 0;
	
}

#include<iostream>
#include<string>

using namespace std;

typedef struct node
{
	char name[20];           //������
	int prio;                      //�������ȼ�
	int round;                   //����CPU��ʱ��Ƭ
	int cputime;                //CPUִ��ʱ��
	int needtime;              //����ִ������ʱ��
	char state;                   //����״̬
	int count;                    //��¼ִ�д���
	struct node* next;       //����ָ��
}PCB;//PCB= struct node
int num;
//�����������У��������У�ִ�ж��У���ɶ���
PCB* ready = NULL;         //��������
PCB* run = NULL;             //ִ�ж���
PCB* finish = NULL;          //��ɶ���
//ȡ�õ�һ�������ڵ�
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
//���ȼ��������
void Output1()
{
	PCB* p;
	p = ready;

	while (p != NULL)
	{
		cout << "�������̣�" << endl;
		cout << p->name << "\t" << p->prio << "\t" << p->cputime << "\t" << p->needtime << "\t " << p->state << "  \t  " << p->count << endl;
		p->prio += 1;
		p = p->next;
	}
	p = finish;
	while (p != NULL)
	{
		cout << "��ɽ��̣�" << endl;
		cout << p->name << "\t" << p->prio << "\t" << p->cputime << "\t" << p->needtime << "\t " << p->state << "  \t  " << p->count << endl;
		p = p->next;
	}
	p = run;
	while (p != NULL)
	{
		cout << "���н��̣�" << endl;
		cout << p->name << "\t" << p->prio << "\t" << p->cputime << "\t" << p->needtime << "\t " << p->state << "  \t  " << p->count << endl;
		p = p->next;
	}
	cout << "****************************************************************" << endl;
}


//�������ȼ����У��涨������ԽС�����ȼ�Խ�� 
void InsertPrio(PCB* in)
{
	PCB* fst, * nxt;
	fst = nxt = ready;

	if (ready == NULL)  //�������Ϊ�գ���Ϊ��һ��Ԫ�� 
	{
		in->next = ready;
		ready = in;
	}
	else     //�鵽���ʵ�λ�ý��в��� 
	{
		if (in->prio >= fst->prio)  //�ȵ�һ����Ҫ������뵽������ͷ
		{
			in->next = ready;
			ready = in;
		}
		else
		{
			while (fst->next != NULL)  //�ƶ�ָ����ҵ�һ������С��Ԫ�ص�λ�ý��в��� 
			{
				nxt = fst;
				fst = fst->next;
			}

			if (fst->next == NULL) //�Ѿ���������β���������ȼ�����С��������뵽��β���� 
			{
				in->next = fst->next;
				fst->next = in;
			}
			else     //���뵽������
			{
				nxt = in;
				in->next = fst;
			}
		}
	}
}
//�����̲��뵽��������β��
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
//�����̲��뵽��ɶ���β��
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
//���ȼ��������뺯�� 
void PrioCreate()
{
	PCB* tmp;
	int i;
	cout << "Enter the name and needtime��" << endl;
	for (i = 0; i < num; i++)
	{
		if ((tmp = (PCB*)malloc(sizeof(PCB))) == NULL)
		{
			cerr << "malloc" << endl;
			exit(1);
		}
		cout << "��������" << endl;
		cin >> tmp->name;
		cout << "��������ʱ�䣺" << endl;
		cin >> tmp->needtime;
		while (tmp->needtime <= 0)
		{
			cout << "����������������룺" << endl;
			cin >> tmp->needtime;
		}
		tmp->cputime = 0;
		tmp->state = 'W';
		tmp->prio = 50 - tmp->needtime;  //���������ȼ�����Ҫ��ʱ��Խ�࣬���ȼ�Խ��
		tmp->round = 0;
		tmp->count = 0;
		InsertPrio(tmp);      //�������ȼ��Ӹߵ��ͣ����뵽�������� 
	}
	cout << "������\t���ȼ�\tcpuʱ��\t��Ҫʱ�� ����״̬ ������" << endl;
}

//�������ȼ����ȣ�ÿ��ִ��һ��ʱ��Ƭ
void Priority()
{
	int flag = 1;
	GetFirst();
	while (run != NULL)
	{
		Output1();
		while (flag)
		{
			run->prio -= 3; //���ȼ���ȥ�� 
			run->cputime++; //CPUʱ��Ƭ��һ 
			run->needtime--;//����ִ����ɵ�ʣ��ʱ���һ 
			if (run->needtime == 0)//�������ִ����ϣ�������״̬��ΪF��������뵽��ɶ��� 
			{
				run->state = 'F';
				run->count++;
				InsertFinish(run);
				flag = 0;
			}
			else   //������״̬��ΪW�����������
			{
				run->state = 'W';
				run->count++; //����ִ�еĴ�����һ
				InsertTime(run);
				flag = 0;
			}
		}
		flag = 1;
		GetFirst();    //����ȡ�������ж�ͷ���̽���ִ�ж��� 
	}
}


int main(void)
{

	int n;
	cout << "������̸�����" << endl;
	cin >> num;
	while (num <= 0)
	{
		cout << "����������������룺" << endl;
		cin >> num;
	}
	cout << "���ȼ�����:" << endl;
	PrioCreate();
	Priority();
	Output1();
	cout << endl;
	system("pause");
	return 0;
	
}

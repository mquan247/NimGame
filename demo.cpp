#include<iostream>
#include<cstdlib>
#include<ctime>
#include <fstream>
using namespace std;
const int maxpile = 5;				// so dong lon nhat co the tao 
const int maxgravel = 7;			// so soi lon nhat trong 1 dong
int n, cur_piles;
//
// Khai bao cau truc du lieu
//
struct gravel_pile{			
   		int gravel;    				// gravel = soi
   		struct gravel_pile *next;
};
//
// Khai bao con tro *List de chi den 1 phan tu kieu gravel_pile
//
typedef gravel_pile *List;	
List root = NULL;					// con tro kieu List chi den dong soi dau tien
//
// Tao n dong soi voi so soi ngau nhien
//
void create(List &F, int n)			
{
	List p = new gravel_pile;      	 // tao dong soi dau tien
   	srand(time(0));
   	p->gravel = rand()%maxgravel+1;  //tao ngau nhien so soi
   	p->next = NULL;
  	F=p;
   	for (int i=1; i<n; i++){      	// tao cac dong soi tiep theo
  		p = new gravel_pile;
       	p->gravel = rand()%maxgravel+1;
   		p->next = F;				// next tro den phan tu tiep theo cua danh sach F
   		F=p;
	}
}
//
// Doc du lieu tu file 
//
void customCreate(List &F,ifstream& fi)		
{
	fi>>n;							// doc so dong soi 
	int x;
	fi>>x;							// dong soi dau tien
	List p, tail ; 
	p = new gravel_pile;			// tao dong soi dau tien
	tail = new gravel_pile;			// tail la con tro cuoi cung cua danh sach F    	
   	srand(time(0));
   	p->gravel = x;					// tao ngau nhien so soi
   	p->next = NULL;
   	F = tail = p;					// phan tu dau tien cua danh sach F
   	for (int i=1; i<n; i++)   		// cac dong soi tiep theo     	
   	{
   			fi>>x;
      		p = new gravel_pile;
       		p->gravel = x;
			p->next=NULL;
			tail->next=p;			//con tro cua tail lien ket voi p 
			tail=p;					//cap nhat lai tail = p
   	}
}
//
// Tong Nim
//
int nim_sum(List root){		
	int s=0;
	List cur=root;
	while(cur!=NULL){
		s=s^cur->gravel;
		cur=cur->next;
	}
	return s;
}
//
// Tao giao dien hien thi
//
void Display(List F){
	List p = F;
	int i=1;
	cout<<"\nDong soi | So luong soi\n";
	while(p !=NULL){
		cout<<"  "<<i<<"      |     "<<p->gravel<<endl;
		p=p->next;
		i++;
	}
}

// Ham kiem tra ket thuc tro choi Nim
bool end(List F){
        List p = F;
        while (p!=NULL){
            if(p->gravel != 0) return false;
            p = p->next;
        }
        return true;
    }

int check_gravel(List root, int k)  //kiem tra dong soi hop le  
{
	List tmp = root;
	for(int i=1; i<k; i++) tmp = tmp->next;
	if (tmp->gravel) return 1;
	else return 0;
}
    
void player_turn(List &F) //Luot di cua nguoi choi    
{
   int  k, n;
   List tmp = root;
   do
   {
   		cout<<"\nMoi ban chon dong soi: "; 
		cin>>k;
   }
   while (k<1 || k >maxpile ||check_gravel(root,k) == 0 );
   for(int i=1;i<k;i++) tmp = tmp->next;
   do
   {
   		cout<<"So soi muon lay: "; 
   		cin>>n;
   }
   while (n<1 || n > tmp->gravel);
   tmp->gravel = tmp->gravel - n;
   if (tmp->gravel == 0) cur_piles--;  //neu boc het thi giam so dong soi
}
//
// Ham tim dong soi co so soi nhieu nhat
//
int findMaxpile(List F, List &max) 
{
	List cur;
	int imax = 1, i = 2;
	max = root;
	cur  = root ->next;
	while(cur!=NULL)
	{
		if(cur->gravel > max->gravel)
		{
			max=cur;
			imax=i;
		}
		cur=cur->next;
		i++;
	}
	return imax;
}
// Trang thai cuoi cung
int final_state(List root) 
{
	int i=0, i1=0, pos;
	List cur=root;
	while(cur!=NULL)
	{
		if(cur->gravel == 1) i1++;
		else if(cur->gravel > 1) pos=i+1;
		cur = cur->next;
		i++;
	}
	if(i1 == (cur_piles-1)) return pos;
	else return 0;
}
//
//Luot di cua may tinh
//
void computer_turn(List &root) 
{
	List tmp = root;
	int take, s, i;
	i = final_state(root);
	// Kiem tra truong hop dac biet
	if(i)       
	{
		for(int j=1;j<i;j++) tmp=tmp->next;
		if(cur_piles%2) 
			take=tmp->gravel - 1 ; 
		else 
			take = tmp->gravel ;
	}
	else{
		s = nim_sum(root);
		i = 1;
		// Neu tong nim khac 0
		if(s) 
		{
			while((tmp->gravel^s) >= tmp->gravel)	//lap cho den khi xuat hien gravel^s < gravel
			{
				tmp = tmp->next; 
				i++;
			}
			take = tmp->gravel - (tmp->gravel^s);
		}
		else 
		{
			i = findMaxpile(root,tmp);
			srand(time(0));
			take = rand()%(tmp->gravel) + 1;
		}
	}
	tmp->gravel -= take;
	if(tmp->gravel == 0) cur_piles--;
	cout<<"\nMay tinh boc "<<take<<" vien o dong soi "<<i;
}

// Ham tung dong xu de quyet dinh luot di
int turn()
{
	int c; 
	srand(time(0));
	c=(rand()%9)%2 ; 
	return c; // 
	
}

void menu()
{
	int m, stt;
	cout<<"+----------------+"<<endl;
	cout<<"|     NIM GAME   |";
	cout<<"\n|1. Normal Mode  |"<<"\n|2. Custom Mode  |"<<"\n|0. Exit         |";
	cout<<"\n+----------------+";
	cout<<"\nSelect mode:";
	cin>>m;
	system("cls");
	switch(m)
	{
		case 1:
			cout<<"Nhap so dong soi ban muon choi (max = 5): ";
			cin>>n;
			cur_piles = n;
			while(n<1 || n>5)
				{
					cout<<"So dong soi 1<=n<=5\n"<<"Moi ban nhap lai: "; 
					cin>>n;
	 			} 
			create(root,n);
			system("cls");
			if(turn()){
					cout<<"Nguoi choi duoc di truoc"; 
					stt=1;
			}
			else{
				cout<<"May tinh duoc di truoc";
				stt=0;
			}
			Display(root);
			while(!end(root)){
				if(stt)	{
					player_turn(root);
					Display(root);
					stt=0;
				}
				else{
					computer_turn(root);
					Display(root);
					stt=1;
				}
			}
			cout<<"\n|--------Ket qua tro choi-------|";
			if(stt)
				cout<<"\n+Nguoi choi thang, may tinh thua+";
			
			else
				cout<<"\n+Nguoi choi thua, may tinh thang+";
				
			break;
		case 2:{ 
			int cnt=1,res;
		    ifstream fi("input.txt");   //open file to read
			ofstream fo("output.txt");  //open file to write	           
			fo<<"+---------------Ket qua tro choi--------------+";	
			while(cnt){
				customCreate(root,fi);	 
		    	cur_piles = n;
				if(turn()){
					cout<<"Nguoi choi duoc di truoc"; 
					stt=1;
				}
				else{
				cout<<"May tinh duoc di truoc";
				stt=0;
				}
				Display(root);
				while(!end(root)){
				if(stt)	{
					player_turn(root);
					Display(root);
					stt=0;
				}
				else{
					computer_turn(root);
					Display(root);
					stt=1;
				}
			}	
			cout<<"\n|--------Ket qua tro choi-------|";
			if(stt)
				cout<<"\n+Nguoi choi thang, may tinh thua+";
			else
				cout<<"\n+Nguoi choi thua, may tinh thang+";
			cout<<"\n+-------------------------------+";
			fo<<"\n|Luot choi "<<cnt<<": ";
			if(stt) fo<<"Nguoi choi thang, may tinh thua |";   
			else fo<<"Nguoi choi thua, may tinh thang |";
			cout<<"\nBan co muon choi tiep?";
			cin>>res;
			cout<<"\n";
			if(res) cnt++;
			else cnt=0;
			}
			fo<<"\n+---------------------------------------------+";
			fi.close();	//close file
			fo.close();	//close file
		}
		break;
		default: break;
	}
}
int main()
{
	menu();
	return 0;
}


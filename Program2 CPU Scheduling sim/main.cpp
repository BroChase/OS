#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

struct PID{
    PID* prev;
    PID* next;

    int _id = 0; //unique id of process
    float _at = 0; //arrival time of process
    float _cpuBurst = 0;  //cpu time requested by process
    int _priority = 0;  //priority of arrival process
    int _context = 0;      //if process exp a context switch
    float _finish = 0;      //calculated when the process finishes
    float _cts = 0;
    float _waitTime = 0;
    float _response = 0;
};

void printF(PID*);              //simple print function to print out the list for testing purposes
PID* createlist(std::string);   //create the list from the input file
void sortListFCFS(PID*);        //sort the list for FCFS "sort the file by the arrival time"
void FCFS(PID*, std::string);                //First come first server alg
void SRTF(PID*, PID*, std::string);          //Shortest remaining time firstalg
int searchSRTF(PID*,PID*, float &, int);   //SRTF search for shortest burst time.
int numberofContext(PID*);      //Calc Number of Context switches.
int searchPP(PID*,PID*,float &, int);
void PP(PID*, PID*, std::string);
void RRQ(PID*, PID*, int, std::string);
int searchRR(PID* ,PID*, float &, int);

//todo testing testing and more testing and exceptions for input.

//*************main*************
int main(int argc, char *argv[]) {
    //std::string file = "input4";
    std::string file = argv[0];
    std::string outputFile = argv[1];
    auto choice = (int)argv[2];
    auto quant = (int)argv[3];
    //create doubly linked list
    //PID* wambat = createlist(file);
    //PID* fpo = createlist(file);
    PID* wambat = createlist(file);
    PID* fpo = createlist(file);
    if(choice == 0){
        FCFS(wambat, outputFile);
    }else if(choice == 1){
        SRTF(wambat, fpo, outputFile);
    }else if(choice == 2){
        RRQ(wambat, fpo, quant, outputFile);
    }else if(choice == 3){
        PP(wambat, fpo, outputFile);
    }
    //sortListFCFS(wambat); //works
    //FCFS(wambat);         //works
    //PP(wambat, fpo);      //works
    //SRTF(wambat, fpo);    //works
    //int testq = 4;
    //RRQ(wambat, fpo, testq);
    return 0;
}
//*************funcs************
PID* createlist(std::string file){
    std::ifstream infile(file);
    std::string line;
    PID* head;
    PID* tail;
    PID* p;
    //todo error check if file was opened. and if objects are created each loop
    //create a new object PID assigning it the head and tail or the list
    //set its prev and next pointers to NULL
    p = new PID;
    head = p;
    tail = p;
    p->prev = nullptr;
    p->next = nullptr;

    std::getline(infile, line);
    std::istringstream iss(line);
    int number;
    iss >> number;
    p->_id = number;
    //std::cout << p->_id << ' ';
    iss >> number;
    p->_at = number;
    //std::cout << p->_at << ' ';
    iss >> number;
    p->_cpuBurst = number;
    //std::cout << p->_cpuBurst << ' ';
    iss >> number;
    p->_priority = number;
    //std::cout << p->_priority << '\n';
    while(std::getline(infile, line)){
        std::istringstream iss(line);
        p = new PID;
        p->prev = tail;
        tail->next = p;
        tail = p;
        p->next = nullptr;

        //set the variables of P
        iss >> number;
        p->_id = number;
        //std::cout << p->_id << ' ';
        iss >> number;
        p->_at = number;
        //std::cout << p->_at << ' ';
        iss >> number;
        p->_cpuBurst = number;
        //std::cout << p->_cpuBurst << ' ';
        iss >> number;
        p->_priority = number;
        //std::cout << p->_priority << '\n';
    }
    return head;
};
void RRQ(PID* head, PID*fpo, int Quant, std::string file){
    PID* current = head;
    PID* checkcurrent = head;
    const char space = ' ';
    const int firstWidth = 6;
    const int secondWidth = 10;
    const int thirdWidth = 16;
    int noc = 0;
    float turnaround,responsetime;
    float avgCPU=0;
    float avgWT=0;
    float avgTAT=0;
    float avgRT=0;
    bool empty = false;
    int numberofProcinq = 0;

    //std::string file = output;
    std::ofstream outfile(file);

    outfile << "*************************************************************************************************************" << '\n';
    outfile << "**********************Scheduling Algorithm: PP ************************************************************" << '\n';
    outfile << "*************************************************************************************************************" << '\n';
    outfile << std::left << std::setw(firstWidth) << std::setfill(space) << "PID";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space)  << "Arrival";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "CPU-burst";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Priority";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Finish";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Waiting Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Turn Around";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Response Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "No. of Context" << '\n';
    PID* contextPrint = head;
    PID* original = fpo;
    PID* OG = fpo;
    int ft = 0;
    do {
        int rem = 0;
        int CP = 1;
        current = head;
        OG = fpo;
        while(current != nullptr) {
            rem = current->_cpuBurst;
            if (current->_at <= ft && current->_cpuBurst != 0 && current->_id == CP) {
                if(current->_cpuBurst == OG->_cpuBurst){
                    noc = numberofContext(head);
                    current->_response = (float)(ft + noc*.5) - current->_at;
                }
                if (current->_cpuBurst < Quant) {
                    current->_cpuBurst = current->_cpuBurst - rem;
                    ft += rem;
                    //if the cpuburst becomes zero during statement then the current has finished
                    //recorde the finish time for the process and add in the amount of context switches
                    if (current->_cpuBurst == 0) {
                        noc = numberofContext(head);
                        current->_finish = (float) (ft + noc * .5);
                    }
                } else {
                    current->_cpuBurst = current->_cpuBurst - Quant;
                    ft += Quant;
                    //if the cpuburst becomes zero during statement then the current has finished
                    //recorde the finish time for the process and add in the amount of context switches
                    if (current->_cpuBurst == 0) {
                        noc = numberofContext(head);
                        current->_finish = (float) (ft + noc * .5);
                    }
                }
                //look at the next process in line to see if it will be run
                //add a context switch
                if (current->next != nullptr) {
                    if (current->next->_at <= ft && current->_cpuBurst != 0) {
                        current->_context++;
                        current->_waitTime += .5;
                    }
                }
                //look at the first process in the list if we are at the list end.
                //if it is not zero then context switch happens back to beginning of list.
                if (current->next == nullptr) {
                    PID *firstCheck = head;
                    if (firstCheck->_cpuBurst != 0 && current->_cpuBurst != 0) {
                        current->_context++;
                        current->_waitTime += .5;
                    }
                }

                PID* print = head;
                while(print != nullptr){
                    std::cout << print->_id << ' ' << print->_cpuBurst << ' ' << print->_context << '\n';
                    print = print->next;
                }
                std::cout<< '\n';
            }
            if(current->next != nullptr) {
                if (current->next->_at <= ft) {
                    CP++;
                }
            }
            OG = OG->next;
            current = current->next;
        }
        std::cout << '\n';

        //check to see if there are any processes still with Burst time
        int finished = 0;
        checkcurrent = head;
        while(checkcurrent != nullptr){
            finished += checkcurrent->_cpuBurst;
            checkcurrent = checkcurrent->next;
        }
        if(finished == 0){
            empty = true;
        }
    }while(!empty);


    while(contextPrint != nullptr) {
        outfile << std::left << std::setw(firstWidth) << std::setfill(space) << contextPrint->_id;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_at;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << original->_cpuBurst;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_priority;
        contextPrint->_waitTime = contextPrint->_finish - (original->_cpuBurst + original->_at);
        turnaround = contextPrint->_waitTime + original->_cpuBurst;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_finish;
        avgCPU += original->_cpuBurst;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_waitTime;
        avgWT += contextPrint->_waitTime;
        avgTAT += turnaround;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << turnaround;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_response;
        avgRT += contextPrint->_response;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_context << '\n';
        contextPrint = contextPrint->next;
        original = original->next;
    }
    current = head;
    while(current != nullptr){
        numberofProcinq += 1;
        current = current->next;
    }
    outfile << std::setw(24) << std::fixed << std::setprecision(2)<< "Average CPU burst time = " << avgCPU/numberofProcinq;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(40) << "Average Waiting Time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgWT/numberofProcinq;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';

    outfile << std::setw(25) << "Average turn around time = " << avgTAT/numberofProcinq;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(38) << std::setfill(space) << "Average response time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgRT/numberofProcinq;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';
    noc = numberofContext(head);
    outfile << "Total No. of Context Switching Performed = " << noc;
}

int searchRR(PID* head,PID* fpo, float &AT, int CP){
    PID* current = head;
    PID* OG = fpo;
    int CS = CP;
    int priority;
    while(current != nullptr){
        if(current->_id == CP) {
            if(current->_cpuBurst == 0){
                current->_finish = (float) (AT);// + current->_cts;//+ (nocs * .5));        //context switches add .5 per switch so nocs *5 + the current arrival time
                current->_waitTime = current->_finish - (current->_at + OG->_cpuBurst);
            }
        }
        OG = OG->next;
        current = current->next;
    }


    current = head;
    priority = 100;
    while (current != nullptr) {
        if (current->_priority < priority && current->_cpuBurst != 0) {
            if(current->_at <= AT){
                priority = current->_priority;
                CP = current->_id;
            }
        }
        current = current->next;
    }

    current = head;
    while(current != nullptr){
        if(current->_id == CP) {
            if(CS != current->_id){     //context switch has occurred
                PID* cont = head;
                while(cont != nullptr){
                    if(cont->_id == CS && cont->_cpuBurst != 0){
                        cont->_context += 1;
                        AT += .5;
                        PID* ad = head;
                        while(ad != nullptr) {      //if the context switch happens add .5 to processes that still have burst times.
                            if (ad->_cpuBurst != 0) {
                                if(ad->_id == current->_id){
                                    ad->_cts = 0;
                                }
                                ad->_cts += .5;
                            }
                            ad = ad->next;
                        }
                    }
                    cont = cont->next;
                }
            }
            AT += .5;
            return current->_id;
        }
        current = current->next;
    }
}

void PP(PID* head, PID* fpo, std::string file){
    PID* current = head;
    PID* checkcurrent = head;
    const char space = ' ';
    const int firstWidth = 6;
    const int secondWidth = 10;
    const int thirdWidth = 16;
    int noc = 0;
    float turnaround,responsetime;
    float avgCPU=0;
    float avgWT=0;
    float avgTAT=0;
    float avgRT=0;
    bool empty = false;
    int numberofProcinq = 0;

    //std::string file = "outputresultsPP";
    std::ofstream outfile(file);

    outfile << "*************************************************************************************************************" << '\n';
    outfile << "**********************Scheduling Algorithm: PP ************************************************************" << '\n';
    outfile << "*************************************************************************************************************" << '\n';
    outfile << std::left << std::setw(firstWidth) << std::setfill(space) << "PID";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space)  << "Arrival";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "CPU-burst";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Priority";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Finish";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Waiting Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Turn Around";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Response Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "No. of Context" << '\n';
    PID* contextPrint = head;
    PID* original = fpo;
    PID* OG = fpo;
    float AT = 0; //set the arrival time == 0;
    int NAT = 0;
    int CP = 1; //sets the starter process 'first process'
    while(contextPrint != nullptr){
        outfile << std::left << std::setw(firstWidth) << std::setfill(space) << contextPrint->_id;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_at;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << original->_cpuBurst;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_priority;
        //get the number of processes in the linked list
        //do while 'run till all processes cpuBurst == 0
        do {
            int finished = 0;
            current = head;
            //use the search to find what process should be processed next based on the AT and _cpuBurst scale
            CP = searchPP(head,fpo,AT,CP);

            OG = fpo;
            current = head;
            while(OG != nullptr){
                if(OG->_id == CP){
                    if(current->_cpuBurst == OG->_cpuBurst){
                        current->_response = (float)((AT-.5) - current->_at);//
                    }
                }
                current = current->next;
                OG = OG->next;
            }
            //decrease the Burst of the CP by 1ms
            for (PID *sub = head; sub != nullptr; sub = sub->next) {
                if (sub->_id == CP) {
                    sub->_cpuBurst = (float)(sub->_cpuBurst - .5);
                }
            }
            checkcurrent = head;
            while(checkcurrent != nullptr){
                finished += checkcurrent->_cpuBurst;
                checkcurrent = checkcurrent->next;
            }
            if(finished == 0){
                empty = true;
            }
        }while(!empty);

        turnaround = contextPrint->_waitTime + original->_cpuBurst;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_finish;
        avgCPU += original->_cpuBurst;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_waitTime;
        avgWT += contextPrint->_waitTime;
        avgTAT += turnaround;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << turnaround;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_response;
        avgRT += contextPrint->_response;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_context << '\n';
        contextPrint = contextPrint->next;
        original = original->next;
    }
    current = head;
    while(current != nullptr){
        numberofProcinq += 1;
        current = current->next;
    }

    outfile << std::setw(24) << std::fixed << std::setprecision(2)<< "Average CPU burst time = " << avgCPU/numberofProcinq;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(40) << "Average Waiting Time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgWT/numberofProcinq;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';

    outfile << std::setw(25) << "Average turn around time = " << avgTAT/numberofProcinq;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(38) << std::setfill(space) << "Average response time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgRT/numberofProcinq;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';
    noc = numberofContext(head);
    outfile << "Total No. of Context Switching Performed = " << noc;
}
int searchPP(PID* head,PID* fpo, float &AT, int CP){
    PID* current = head;
    PID* OG = fpo;
    int CS = CP;
    int priority;
    while(current != nullptr){
        if(current->_id == CP) {
            if(current->_cpuBurst == 0){
                current->_finish = (float) (AT);// + current->_cts;//+ (nocs * .5));        //context switches add .5 per switch so nocs *5 + the current arrival time
                current->_waitTime = current->_finish - (current->_at + OG->_cpuBurst);
                }
            }
        OG = OG->next;
        current = current->next;
    }


    current = head;
    priority = 100;
    while (current != nullptr) {
        if (current->_priority < priority && current->_cpuBurst != 0) {
            if(current->_at <= AT){
                priority = current->_priority;
                CP = current->_id;
            }
        }
        current = current->next;
    }

    current = head;
    while(current != nullptr){
        if(current->_id == CP) {
            if(CS != current->_id){     //context switch has occurred
                PID* cont = head;
                while(cont != nullptr){
                    if(cont->_id == CS && cont->_cpuBurst != 0){
                        cont->_context += 1;
                        AT += .5;
                        PID* ad = head;
                        while(ad != nullptr) {      //if the context switch happens add .5 to processes that still have burst times.
                            if (ad->_cpuBurst != 0) {
                                if(ad->_id == current->_id){
                                    ad->_cts = 0;
                                }
                                ad->_cts += .5;
                            }
                            ad = ad->next;
                        }
                    }
                    cont = cont->next;
                }
            }
            AT += .5;
            return current->_id;
        }
        current = current->next;
    }
}


void SRTF(PID* head, PID* fpo, std::string file){
    PID* current = head;
    PID* checkcurrent = head;
    const char space = ' ';
    const int firstWidth = 6;
    const int secondWidth = 10;
    const int thirdWidth = 16;
    int noc = 0;
    float turnaround,responsetime;
    float avgCPU=0;
    float avgWT=0;
    float avgTAT=0;
    float avgRT=0;
    bool empty = false;
    int numberofProcinq = 0;

    //std::string file = "outputresultsSRTF";
    std::ofstream outfile(file);

    outfile << "*************************************************************************************************************" << '\n';
    outfile << "**********************Scheduling Algorithm: SRTF ************************************************************" << '\n';
    outfile << "*************************************************************************************************************" << '\n';
    outfile << std::left << std::setw(firstWidth) << std::setfill(space) << "PID";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space)  << "Arrival";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "CPU-burst";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Priority";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Finish";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Waiting Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Turn Around";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Response Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "No. of Context" << '\n';
    PID* contextPrint = head;
    PID* original = fpo;
    PID* OG = fpo;
    float AT = 0; //set the arrival time == 0;
    int CP = 1; //sets the starter process 'first process'
    while(contextPrint != nullptr){
        outfile << std::left << std::setw(firstWidth) << std::setfill(space) << contextPrint->_id;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_at;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << original->_cpuBurst;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_priority;
    //get the number of processes in the linked list
    //do while 'run till all processes cpuBurst == 0
    do {
        int finished = 0;
        current = head;
        //use the search to find what process should be processed next based on the AT and _cpuBurst scale

        CP = searchSRTF(head,fpo,AT,CP);

        OG = fpo;
        current = head;
        while(OG != nullptr){
            if(OG->_id == CP){
                if(current->_cpuBurst == OG->_cpuBurst){
                    current->_response = (float)((AT-.5) - current->_at);//
                }
            }
            current = current->next;
            OG = OG->next;
        }
        //decrease the Burst of the CP by 1ms
        for (PID *sub = head; sub != nullptr; sub = sub->next) {
            if (sub->_id == CP) {
                sub->_cpuBurst = (float)(sub->_cpuBurst - .5);
            }
        }
        checkcurrent = head;
        while(checkcurrent != nullptr){
            finished += checkcurrent->_cpuBurst;
            checkcurrent = checkcurrent->next;
        }
        if(finished == 0){
            empty = true;
        }
    }while(!empty);

        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_finish;
        avgCPU += original->_cpuBurst;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_waitTime;
        avgWT += contextPrint->_waitTime;
        turnaround = contextPrint->_waitTime + original->_cpuBurst;
        avgTAT += turnaround;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << turnaround;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_response;
        avgRT += contextPrint->_response;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_context << '\n';
        contextPrint = contextPrint->next;
        original = original->next;
    }
    current = head;
    while(current != nullptr){
        numberofProcinq += 1;
        current = current->next;
    }

    outfile << std::setw(24) << std::fixed << std::setprecision(2)<< "Average CPU burst time = " << avgCPU/numberofProcinq;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(40) << "Average Waiting Time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgWT/numberofProcinq;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';

    outfile << std::setw(25) << "Average turn around time = " << avgTAT/numberofProcinq;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(38) << std::setfill(space) << "Average response time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgRT/numberofProcinq;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';
    noc = numberofContext(head);
    outfile << "Total No. of Context Switching Performed = " << noc;
};
int searchSRTF(PID* head,PID* fpo,float &AT, int CP){
    PID* current = head;
    float shortBurst = 0;
    int CS = CP;
    PID* OG = fpo;

    while(current != nullptr){
        if(current->_id == CP) {
            shortBurst = current->_cpuBurst;
            if(current->_cpuBurst == 0){
                current->_finish = (float) (AT);// + current->_cts;//+ (nocs * .5));        //context switches add .5 per switch so nocs *5 + the current arrival time
                current->_waitTime = current->_finish - (current->_at + OG->_cpuBurst);
                PID* newcurrent = head;
                while(newcurrent != nullptr){
                    if(newcurrent->_cpuBurst != 0){
                        shortBurst = newcurrent->_cpuBurst;
                        CP = newcurrent->_id;
                    }
                    newcurrent = newcurrent->next;
                }
            }
        }
            OG = OG->next;
            current = current->next;
    }

    current = head;
    while (current != nullptr) {
        if (current->_cpuBurst < shortBurst && current->_at <= AT && current->_cpuBurst != 0) {
            shortBurst = current->_cpuBurst;
        }
        current = current->next;
    }

    current = head;
    while(current != nullptr){
        if(current->_cpuBurst == shortBurst) {
            if(CS != current->_id){     //context switch has occurred
                PID* cont = head;
                while(cont != nullptr){
                    if(cont->_id == CS && cont->_cpuBurst != 0){
                        cont->_context +=1;
                        AT += .5;
                        PID* ad = head;
                        while(ad != nullptr) {      //if the context switch happens add .5 to processes that still have burst times.
                            if (ad->_cpuBurst != 0) {
                                if(ad->_id == current->_id){
                                    ad->_cts = 0;
                                }
                                ad->_cts += .5;
                            }
                            ad = ad->next;
                        }
                    }
                    cont = cont->next;
                }
            }
            AT += .5;
            return current->_id;
        }
        current = current->next;
    }
}


void sortListFCFS(PID* head){
    PID* current = head;
    auto *temp = new PID;
    //printF(current);

    int swapped = true;
    while(swapped){
        swapped = false;

        current = head;
        while(current->next != nullptr){
            if(current->_at > current->next->_at){
                swapped = true;
                temp->_id = current->_id;
                current->_id = current->next->_id;
                current->next->_id = temp->_id;

                temp->_at = current->_at;
                current->_at = current->next->_at;
                current->next->_at = temp->_at;

                temp->_cpuBurst = current->_cpuBurst;
                current->_cpuBurst = current->next->_cpuBurst;
                current->next->_cpuBurst = temp->_cpuBurst;

                temp->_priority = current->_priority;
                current->_priority = current->next->_priority;
                current->next->_priority = temp->_priority;
            }
            current = current->next;
        }
    }
};
void FCFS(PID* head, std::string file){
    PID* temp = head;
    const char space = ' ';
    const int firstWidth = 6;
    const int secondWidth = 10;
    const int thirdWidth = 16;
    int noc = 0;
    float finish,waitingtime,turnaround,responsetime;
    float prevFinish=0;
    float avgCPU=0;
    float avgWT=0;
    float avgTAT=0;
    float avgRT=0;
    int burstCount=0;
    int numofProc=0;
    //std::string file = "outputresultsFCFS";
    std::ofstream outfile(file);
    outfile << "*************************************************************************************************************" << '\n';
    outfile << "**********************Scheduling Algorithm: FCFS ************************************************************" << '\n';
    outfile << "*************************************************************************************************************" << '\n';
    outfile << std::left << std::setw(firstWidth) << std::setfill(space) << "PID";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space)  << "Arrival";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "CPU-burst";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Priority";
    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Finish";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Waiting Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Turn Around";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Response Time";
    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "No. of Context" << '\n';

    while(temp != nullptr){
        outfile << std::left << std::setw(firstWidth) << std::setfill(space) << temp->_id;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space)  << temp->_at;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << temp->_cpuBurst;
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << temp->_priority;
        if(temp->_at == 0){
            burstCount += temp->_cpuBurst;
            finish = temp->_cpuBurst;
        } else{
            if(temp->_at < burstCount){
                burstCount += temp->_cpuBurst;
                finish = burstCount;
            } else
                finish = temp->_cpuBurst;
        }
        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << finish;
        avgCPU =+ burstCount;
        //waitingtime = previous processes finish - the arrival time of process.
        waitingtime = prevFinish - temp->_at;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << waitingtime;
        avgWT += waitingtime;
        //turnaround = finish - arrival time
        turnaround = finish - temp->_at;
        avgTAT += turnaround;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << turnaround;
        //responsetime = turnaround - burst
        responsetime = turnaround - temp->_cpuBurst;
        avgRT += responsetime;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << responsetime;
        //no NOC with FCFS
        noc = 0;
        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << noc << '\n';
        prevFinish = finish;
        numofProc ++;
        temp = temp->next;
    }

    outfile << std::setw(24) << std::fixed << std::setprecision(2)<< "Average CPU burst time = " << avgCPU/numofProc;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(40) << "Average Waiting Time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgWT/numofProc;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';

    outfile << std::setw(25) << "Average turn around time = " << avgTAT/numofProc;
    outfile << std::left << std::setw(2)<< " ms";
    outfile << std::right << std::setw(38) << std::setfill(space) << "Average response time = ";
    outfile << std::right << std::setw(5) << std::setfill(space) << avgRT/numofProc;
    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';
    outfile << "Total No. of Context Switching Performed = " << noc;
};

int numberofContext(PID* head){
    int count = 0;
    PID* current = head;
    while(current != nullptr){
        if(current->_context > 0){
            count += current->_context;
        }
        current = current->next;
    }
    return count;
}
void printF(PID* head){
    PID* temp = head;

    while(temp != nullptr){
        std::cout << temp->_id << ' ';
        std::cout << temp->_at << ' ';
        std::cout << temp->_cpuBurst << ' ';
        std::cout << temp->_priority << '\n';
        temp = temp->next;
    }

};


//void RRQ(PID* head, PID*fpo, int Quant){
//    PID* current = head;
//    PID* checkcurrent = head;
//    const char space = ' ';
//    const int firstWidth = 6;
//    const int secondWidth = 10;
//    const int thirdWidth = 16;
//    int noc = 0;
//    float turnaround,responsetime;
//    float avgCPU=0;
//    float avgWT=0;
//    float avgTAT=0;
//    float avgRT=0;
//    bool empty = false;
//    int numberofProcinq = 0;
//
//    std::string file = "outputresultsRR1";
//    std::ofstream outfile(file);
//
//    outfile << "*************************************************************************************************************" << '\n';
//    outfile << "**********************Scheduling Algorithm: RR ************************************************************" << '\n';
//    outfile << "*************************************************************************************************************" << '\n';
//    outfile << std::left << std::setw(firstWidth) << std::setfill(space) << "PID";
//    outfile << std::left << std::setw(secondWidth) << std::setfill(space)  << "Arrival";
//    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "CPU-burst";
//    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Priority";
//    outfile << std::left << std::setw(secondWidth) << std::setfill(space) << "Finish";
//    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Waiting Time";
//    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Turn Around";
//    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "Response Time";
//    outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << "No. of Context" << '\n';
//    PID* contextPrint = head;
//    PID* original = fpo;
//    PID* OG = fpo;
//    int CP = 1;
//    float AT = 0;
//    float nac = 0;
//    float nacp = 0;
//
//    std::ifstream infile(file);
//    std::string line;
//    PID* front;
//    PID* tail;
//    PID* p;
//    //todo error check if file was opened. and if objects are created each loop
//    //create a new object PID assigning it the head and tail or the list
//    //set its prev and next pointers to NULL
//    p = new PID;
//    front = p;
//    tail = p;
//    p->prev = nullptr;
//    p->next = nullptr;
//
//    //first process arrives and gets put into the ready que
//    current = head;
//    while(current != nullptr){
//        if(current->_at <= AT){
//            p = new PID;
//            p->prev = tail;
//            tail->next = p;
//            tail = p;
//            p->next = nullptr;
//            p->_id = current->_id;
//            p->_at = current->_at;
//            p->_cpuBurst = current->_cpuBurst;
//            p->_priority = current->_priority;
//            front = p; //set the head of the que.
//        }
//        current = current->next;
//    }
//    nac += Quant;
//    bool newprocess = false;
//    bool newprocess2 = false;
//    PID* run = front;  //ready que
//    nac = AT;
//    do{
//        PID* joker = head;
//        while(joker != nullptr){
//            if(joker->_at <= (AT) && joker->_cpuBurst != 0 && run->_id != joker->_id){
//                PID* pcc = front;
//                newprocess = true;
//                newprocess2 = true;
//                while(pcc != nullptr){
//                    if(pcc->_id == joker->_id){ //process already in the q
//                        newprocess = false;
//                        newprocess2 = false;
//                    }
//                    pcc = pcc->next;
//                }
//                p = new PID;
//                p->next = nullptr;      //if the process arrives place it in the Rq.
//                p->_id = joker->_id;
//                p->_at = joker->_at;
//                p->_cpuBurst = joker->_cpuBurst;
//                p->_priority = joker->_priority;
//            }
//            if(newprocess){
//                tail->next = p;
//                p->next = nullptr;
//                p->prev = tail;
//                tail = p;
//                newprocess = false;
//            }
//            if(newprocess2 || AT == 0){        //move the process to end of the Rq when it finishes.
//                //AT += .5;
//                auto temp = new PID;
//                temp->next = front;
//                temp->_id = front->_id;
//                temp->_at = front->_at;
//                temp->_cpuBurst = front->_cpuBurst;
//                //remove quant time from the process at head of Que
//                if(temp->_cpuBurst < Quant){
//                    AT += temp->_cpuBurst;
//                    temp->_cpuBurst = 0;
//                    PID* newhead = head;
//                    while(newhead != nullptr){
//                        if(newhead->_id == temp->_id){
//                            newhead->_finish = AT;
//                        }
//                        newhead = newhead->next;
//                    }
//                }else{
//                    AT += Quant;
//                    temp->_cpuBurst = temp->_cpuBurst-Quant;
//                    if(temp->_cpuBurst ==0){
//                        PID* newhead = head;
//                        while(newhead != nullptr){
//                            if(newhead->_id == temp->_id){
//                                newhead->_finish = (AT-2);
//                                newhead->_cpuBurst = 0;
//                            }
//                            newhead = newhead->next;
//                        }
//                    }
//                }
//                std::cout<< '\n';
//                std::cout << temp->_id << ' ' << temp->_cpuBurst << '\n';
//                if(front->next != nullptr){
//                    //AT += .5;
//                    front = front->next;
//                    front->prev = nullptr;
//                    tail->next = temp;
//                    temp->prev = tail;
//                    tail = temp;
//                    tail->next = nullptr;
//                }
//            }
//            joker = joker->next;
//        }
//        std::cout << '\n';
//        run = front;
//        int finished = 0;
//        checkcurrent = head;
//        while(checkcurrent != nullptr){
//            finished += checkcurrent->_cpuBurst;
//            checkcurrent = checkcurrent->next;
//        }
//        if(finished == 0){
//            empty = true;
//        }
//    }while(!empty);
//
//    while(contextPrint != nullptr) {
//        outfile << std::left << std::setw(firstWidth) << std::setfill(space) << contextPrint->_id;
//        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_at;
//        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << original->_cpuBurst;
//        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_priority;
//        contextPrint->_waitTime = contextPrint->_finish - (original->_cpuBurst + original->_at);
//        turnaround = contextPrint->_waitTime + original->_cpuBurst;
//        outfile << std::left << std::setw(secondWidth) << std::setfill(space) << contextPrint->_finish;
//        avgCPU += original->_cpuBurst;
//        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_waitTime;
//        avgWT += contextPrint->_waitTime;
//        avgTAT += turnaround;
//        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << turnaround;
//        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_response;
//        avgRT += contextPrint->_response;
//        outfile << std::left << std::setw(thirdWidth) << std::setfill(space) << contextPrint->_context << '\n';
//        contextPrint = contextPrint->next;
//        original = original->next;
//    }
////    current = head;
////    while(current != nullptr){
////        numberofProcinq += 1;
////        current = current->next;
////    }
//    outfile << std::setw(24) << std::fixed << std::setprecision(2)<< "Average CPU burst time = " << avgCPU/numberofProcinq;
//    outfile << std::left << std::setw(2)<< " ms";
//    outfile << std::right << std::setw(40) << "Average Waiting Time = ";
//    outfile << std::right << std::setw(5) << std::setfill(space) << avgWT/numberofProcinq;
//    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';
//
//    outfile << std::setw(25) << "Average turn around time = " << avgTAT/numberofProcinq;
//    outfile << std::left << std::setw(2)<< " ms";
//    outfile << std::right << std::setw(38) << std::setfill(space) << "Average response time = ";
//    outfile << std::right << std::setw(5) << std::setfill(space) << avgRT/numberofProcinq;
//    outfile << std::left << std::setw(6) << std::setfill(space) << " ms" << '\n';
//    noc = numberofContext(head);
//    outfile << "Total No. of Context Switching Performed = " << noc;
//}
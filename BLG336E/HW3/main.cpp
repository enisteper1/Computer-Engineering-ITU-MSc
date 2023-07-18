#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include <utility>

using namespace std;

struct Job
{
    string job_name;
    int starting_time;
    int ending_time;
    int value;
};

struct Asset {
    string name;
    double price;
    double value;

};

class Scheduler
{
    public:
        // to iterate jobs
        int jobcount;
        // total revenue
        int total_revenue;
        // parent directory of case
        string parent_dir;
        map<string, int> base_place_values;
        map<string, map<int, map<int, int>>> date_based_values;
        // placeholders to hold information that is read from txt files
        // it was required to print outputs in the order of inputs
        vector<string> placenames_for_schedules;
        vector<Asset> assets;
        map<string, vector<pair<int, int>>> place_dates;
        map<string, map<string, vector<pair<int, int>>>> times;
        map<string, map<string, int>> capacities;
        // functions to read txt
        void read_assets(fstream&, string);
        void read_place_dates(fstream&, string);
        void read_capacity(fstream&, string);
        void read_schedule(fstream&, string);
        bool compare_job_times(Job, Job);
        vector<Job> calculate_best_combination(Job*, int, int*);
        int get_previous_available_job(Job*, int);
        pair<int, int*> calculate_max_value(Job*, int);
        Job* init_time_jobs(string);
        void date_based_value_calculation();
        // calculates best times for each place
        void find_best_times();
        // calculates best tour
        void find_best_tour();
        // selects best valuable combination
        void upgrade_list();
        // decode time to write txt
        string time_decoding(int);
        // decode date 
        string convert_to_date(int);
        int convert_to_total_days(const string&);

};

// calculate values for each tour with respect to their total days
void Scheduler::date_based_value_calculation()
{
    int value;
    vector<Job> jobVector;
    for(auto &each_date : place_dates) {
        string placeName = each_date.first;
        vector<pair<int, int>> &tmp_vec = each_date.second;
        
        for(auto &each_pair : tmp_vec) {
            value = base_place_values[placeName] * (each_pair.second - each_pair.first);
            date_based_values[placeName][each_pair.first][each_pair.second] = value;
        }
    }
}
string Scheduler::time_decoding(int input_time)
{
    int hour, minute;
    string decoded_hour, decoded_minute;
    hour = input_time / 60;
    minute = input_time % 60;
    if(hour == 0)
        decoded_hour = "00";
    else
        decoded_hour = to_string(hour);

    if(minute == 0)
        decoded_minute = "00";
    else if(minute < 10)
        decoded_minute = "0" + to_string(minute);
    else
        decoded_minute = to_string(minute);
    
    
    return decoded_hour + ":" + decoded_minute; 
}

void Scheduler::find_best_times()
{
    // string to write txt
    string total_str = "";
    // txt file
    ofstream times_txt;
    times_txt.open ("outputs/" + parent_dir + "/best_for_eachplace.txt");
    // for each place run weighted job scheduling algorithm to calculate best times combination
    for(size_t i=0; i < placenames_for_schedules.size(); i++)
    {
        // initalize times and saloon names for each place
        Job* jobs = init_time_jobs(placenames_for_schedules[i]);
        // calculate the max obtainable value from jobs
        pair<int, int*> result = calculate_max_value(jobs, jobcount);
        // 
        int max_value = result.first;
        int* jobSchedule = result.second;
        base_place_values[placenames_for_schedules[i]] = max_value;
        // in order to obtain optimal jobs
        vector<Job> optimalJobs = calculate_best_combination(jobs, jobcount, jobSchedule);
        total_str += placenames_for_schedules[i] +  " --> " + to_string(max_value) + "\n";
        for (const Job& job : optimalJobs) 
            total_str += placenames_for_schedules[i] + "\t" + job.job_name + "\t\t" + time_decoding(job.starting_time) + "\t" + time_decoding(job.ending_time) + "\n";
        
        total_str += "\n";
    }
    times_txt << total_str;
    times_txt.close();

}

// assign values with respect to saloon capacities
Job* Scheduler::init_time_jobs(string placename)
{
    vector<Job> jobVector;
    for(auto &tmp_time : times[placename]) {
        string saloon_name = tmp_time.first;
        vector<pair<int, int>> &time_vector = tmp_time.second;

        for(auto &each_time : time_vector) {
            // job informations
            Job job;
            job.job_name = saloon_name;
            job.starting_time = each_time.first;
            job.ending_time = each_time.second;
            job.value = capacities[placename][saloon_name];

            jobVector.push_back(job);
        }
    }
    

    // Now you can convert the vector to an array
    jobcount = jobVector.size();
    Job* jobArray = new Job[jobcount];

    for(int i = 0; i < jobcount; ++i)
        jobArray[i] = jobVector[i];

    return jobArray;
}

// finds best tour for eachg date
void Scheduler::find_best_tour()
{
    // calculates date based values for each placename & date
    date_based_value_calculation();
    vector<Job> jobVector;
    for(auto &date : place_dates) {
        string placeName = date.first;
        vector<pair<int, int>> &tmp_vec = date.second;
        
        for(auto &each_pair : tmp_vec) {
            Job job;
            job.job_name = placeName;
            job.starting_time = each_pair.first;
            job.ending_time = each_pair.second;
            job.value = date_based_values[placeName][each_pair.first][each_pair.second];

            jobVector.push_back(job);
        }
    }

    jobcount = jobVector.size();
    Job* jobArray = new Job[jobcount];

    for(int i = 0; i < jobcount; ++i)
        jobArray[i] = jobVector[i];

    string total_str = "";
    ofstream tours_txt;
    tours_txt.open("outputs/" + parent_dir + "/best_tour.txt");

    pair<int, int*> result = calculate_max_value(jobArray, jobcount);
    int max_value = result.first;
    int* jobSchedule = result.second;
    total_revenue = max_value;
    total_str += "Total Revenue --> " + to_string(max_value) + "\n";
    vector<Job> optimalJobs = calculate_best_combination(jobArray, jobcount, jobSchedule);
    for (const Job& job : optimalJobs)
        total_str += job.job_name + "\t\t" + convert_to_date(job.starting_time) + "\t" + convert_to_date(job.ending_time) + "\n";
    
    tours_txt << total_str;
    tours_txt.close();

}

void Scheduler::read_assets(fstream& file, string delimiter)
{
    // to get parsed str 
    size_t pos = 0;
    // placeholders
    string line, parsed;
    Asset asset;
    // umap key
    string key;
    // to check is first line
    bool is_first = true;
    // to check is first substring
    bool is_first_parse = true;
    while (getline(file, line))
    {
        if(is_first)
        {
            is_first = false;
            continue;
        }

        is_first_parse = true;
        while ((pos = line.find(delimiter)) != string::npos)
        {
            // parse str
            parsed = line.substr(0, pos);
            if(is_first_parse)
            {
                asset.name = parsed;
                is_first_parse = false;
            }
            else
                asset.price = stod(parsed);
            line.erase(0, pos + delimiter.length());
        }
        asset.value = stod(line);
        assets.push_back(asset);
    }

}
int Scheduler::convert_to_total_days(const string& date) {
    stringstream ss(date);
    int day, month;
    char dot;
    // for instance 15.04
    ss >> day >> dot >> month;
    int total_days_passed = 0;
    int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // add each month to calculate total days passed
    for (int i = 0; i < month - 1; ++i) {
        total_days_passed += month_days[i];
    }
    total_days_passed += day;

    return total_days_passed;
}

string Scheduler::convert_to_date(int total_days_passed) {
    vector<string> months = {"January", "February", "March",
                                "April", "May", "June", 
                              "July", "August", "September",
                              "October", "November", "December"};
    int month_days[12] = {31, 28, 31,
                          30, 31, 30,
                          31, 31, 30,
                          31, 30, 31};
    int month = 0;

    while (total_days_passed > month_days[month]) {
        total_days_passed -= month_days[month];
        ++month;
    }

    return months[month] + " " + to_string(total_days_passed);
}
// read place_dates
void Scheduler::read_place_dates(fstream& file, string delimiter)
{
    // to get parsed str 
    size_t pos = 0;
    // placeholders
    string line, parsed, parsed_date;
    // int placeholder for date
    int  starting_date, ending_date;
    // umap key
    string key;
    // to check is first line
    bool is_first = true;
    // to check is first substring
    bool is_first_parse = true;
    // date delimiter
    string date_delimiter = ".";
    while (getline(file, line))
    {
        if(is_first)
        {
            is_first = false;
            continue;
        }

        is_first_parse = true;
        while ((pos = line.find(delimiter)) != string::npos)
        {
            // parse str
            parsed = line.substr(0, pos);
            if(is_first_parse)
            {
                key = parsed;
                is_first_parse = false;
            }
            else
            {
                starting_date = convert_to_total_days(parsed);
            }
            line.erase(0, pos + delimiter.length());
        }

        ending_date = convert_to_total_days(line);
        place_dates[key].push_back(make_pair(starting_date, ending_date));
    }
}


void Scheduler::read_capacity(fstream& file, string delimiter)
{
    // to get parsed str 
    size_t pos = 0;
    // placeholders
    string line, parsed;
    // umap key
    string key1, key2;
    // to check is first line
    bool is_first = true;
    // to check is first substring
    bool is_first_parse = true;
    while (getline(file, line))
    {
        if(is_first)
        {
            is_first = false;
            continue;
        }

        is_first_parse = true;
        while ((pos = line.find(delimiter)) != string::npos)
        {
            // parse str
            parsed = line.substr(0, pos);
            if(is_first_parse)
            {
                key1 = parsed;
                is_first_parse = false;
            }
            else
                key2 = parsed;
            line.erase(0, pos + delimiter.length());
        }
        capacities[key1][key2] = stoi(line);
    }
}

void Scheduler::read_schedule(fstream& file, string delimiter)
{
    // to get parsed str 
    size_t pos = 0;
    // placeholders
    string line, parsed, parsed_time;
    // int placeholder for date
    int hour, minute, starting_time, ending_time;
    // umap key
    string placename, job_name;
    // to check is first line
    bool is_first = true;
    // to check is first substring
    int parse_cnt = 0;
    // date delimiter
    string time_delimiter = ":";
    while (getline(file, line))
    {
        if(is_first)
        {
            is_first = false;
            continue;
        }

        parse_cnt = 0;
        while ((pos = line.find(delimiter)) != string::npos)
        {
            // parse str
            parsed = line.substr(0, pos);
            if(parse_cnt == 0)
            {
                placename = parsed;
                if (find(placenames_for_schedules.begin(), placenames_for_schedules.end(), placename) == placenames_for_schedules.end()) {
                    // If 'str' is not in 'vec', add it
                    placenames_for_schedules.push_back(placename);
                }
            }
            else if (parse_cnt == 1)
                job_name = parsed;
            else
            {
                parsed_time = parsed.substr(0, parsed.find(time_delimiter));
                hour = 60 * stoi(parsed_time);
                parsed.erase(0, parsed.find(time_delimiter) + time_delimiter.length());
                //cout<<placename<<"---"<<parsed_time<<"--"<<parsed<<endl;
                minute = stoi(parsed);
                starting_time = hour + minute;
            }
            line.erase(0, pos + delimiter.length());
            parse_cnt++;
        }
        parsed_time = line.substr(0, line.find(time_delimiter));
        hour = 60 * stoi(parsed_time);
        line.erase(0, line.find(time_delimiter) + time_delimiter.length());
        minute = stoi(line);
        ending_time = hour + minute;
        times[placename][job_name].push_back(make_pair(starting_time, ending_time));
    }
}

// compare jobs ending time for sort operation
bool Scheduler::compare_job_times(Job first_job, Job second_job) {
    return (first_job.ending_time < second_job.ending_time);
}

// gets the previous available job to write it txt
int Scheduler::get_previous_available_job(Job job_arr[], int i) {
    
    for (int j=i-1; j>=0; j--) {

        if (job_arr[j].ending_time <= job_arr[i].starting_time)
            return j;
    }
    return -1;
}

// calculates best combination of jobs with get_previous_available job function
vector<Job> Scheduler::calculate_best_combination(Job job_arr[], int n, int* jobSchedule) {
    vector<Job> jobs;
    int i = n-1;
    // while there is another compatible job continue to iterate
    while(i >= 0) {
        if(i == 0 || job_arr[i].value + jobSchedule[get_previous_available_job(job_arr, i)] > jobSchedule[i-1]) {
            jobs.push_back(job_arr[i]);
            i = get_previous_available_job(job_arr, i);
        }
        else {
            --i;
        }
    }
    reverse(jobs.begin(), jobs.end());
    return jobs;
}

// calculates max values for both times and place_dates since their approach is same
pair<int, int*> Scheduler::calculate_max_value(Job job_arr[], int n) {
    sort(job_arr, job_arr+n, [this](Job first_job, Job second_job) { return compare_job_times(first_job, second_job); });
  
    int *jobSchedule = new int[n];
    jobSchedule[0] = job_arr[0].value;

    for (int i=1; i<n; i++) {
        int inclProf = job_arr[i].value;
        int l = get_previous_available_job(job_arr, i);
        if (l != -1)
            inclProf += jobSchedule[l];

        jobSchedule[i] = max(inclProf, jobSchedule[i-1]);
    }

    return {jobSchedule[n-1], jobSchedule};
}

// 0/1 Knapsack algorithm implementation
void Scheduler::upgrade_list() {
    vector<vector<double>> max_values(assets.size()+1, vector<double>(total_revenue+1, 0));
    vector<vector<bool>> item_holder(assets.size()+1, vector<bool>(total_revenue+1, false));

    // knapsack algorithm
    for (size_t i = 1; i <= assets.size(); ++i) {
        for (int w = 0; w <= total_revenue; ++w) {
            if (assets[i-1].price <= w) {
                if (assets[i-1].value + max_values[i-1][w-assets[i-1].price] > max_values[i-1][w]) {
                    max_values[i][w] = assets[i-1].value + max_values[i-1][w-assets[i-1].price];
                    item_holder[i][w] = true;
                } else {
                    max_values[i][w] = max_values[i-1][w];
                }
            } else {
                max_values[i][w] = max_values[i-1][w];
            }
        }
    }
    // to write selected assets
    vector<Asset> selected_assets;
    // add to vector via backtrack
    for (int i = assets.size(), w = total_revenue; i > 0; --i) {
        if (item_holder[i][w]) {
            selected_assets.push_back(assets[i-1]);
            w -= assets[i-1].price;
        }
    }

    
    // write to txt
    ofstream upgrades_txt("outputs/" + parent_dir + "/upgrade_list.txt");
    double total_value = 0;
    string total_str = "";
    for (const auto& asset : selected_assets) {
        total_str +=  asset.name + "\n";
        total_value += asset.value;
    }
    total_str = "Total Value --> " + to_string(total_value) + "\n" + total_str;
    upgrades_txt << total_str;
    upgrades_txt.close();
}

int main(int argc, char *argv[]){

    // Check input path is passed
    if(argc <= 1)
    {
        cout<<"Case no should be passed!"<<endl;
        return 0;
    }
    // Read Input path
    // assets.txt
    string case_no = argv[1];
    string filename = "inputs/" + case_no + "/assets.txt";
    fstream assets(filename);
    // availability_intervals.txt
    filename = "inputs/" + case_no + "/availability_intervals.txt";
    fstream availability_intervals(filename);
    // capacity.txt
    filename = "inputs/" + case_no + "/capacity.txt";
    fstream capacity(filename);
    // daily_schedule.txt
    filename = "inputs/" + case_no + "/daily_schedule.txt";
    fstream daily_schedule(filename);

    // Delimiter
    string delimiter = " ";
    string dir_delimiter =  "/";
    string tmp_arg = argv[1];
    tmp_arg.erase(0, tmp_arg.find(dir_delimiter) + dir_delimiter.length());
    Scheduler scheduler;
    // assign the parent directory, such as case_1
    scheduler.parent_dir = tmp_arg.substr(0, tmp_arg.find(dir_delimiter));
    // read txt files
    scheduler.read_assets(assets, delimiter);
    scheduler.read_place_dates(availability_intervals, delimiter);
    scheduler.read_capacity(capacity, delimiter);
    scheduler.read_schedule(daily_schedule, delimiter);
    // find best time combination for each place
    scheduler.find_best_times();
    // find best tour for each case
    scheduler.find_best_tour();
    // find most valuable items for each case
    scheduler.upgrade_list();


    return 0;
}


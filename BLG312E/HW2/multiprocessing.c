#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <sys/mman.h> 
#include <sys/wait.h>
#include <sys/time.h> 
// total customer and product number
#define NUM_CUSTOMERS 3
#define NUM_PRODUCTS 5

sem_t *sem_lock;

// to make linkedlist structure of ordered and purchased products
struct Ordered_Product
{
	int product_ID;
	int product_quantity;
	struct Ordered_Product* next;
};

// Customer information
struct Customer
{
	int customer_ID;
	double customer_Balance;
	//Initial balance is used to print before and after shopping difference
	double initial_balance;
	// linkedlist to show ordered items
	struct Ordered_Product* ordered_items;
	// linkedlist to show purchased items
	struct Ordered_Product* purchased_items;
};

// Product information
struct Product
{
	int product_ID;
	double product_Price;
	int product_Quantity;
	int initial_quantity;
};

// defined products and customers globally to reach by functions
// also products information are defined as shared to modify at update every process simulatneously
struct Product *products;
struct Customer customers[NUM_CUSTOMERS];

// prints initial product informations
void print_initial_products(int customer_id)
{
	printf("Customer%d:\nInitial Products:\nProduct ID\tQuantity\tPrice\t\n", customer_id);
	int i;
	for(i=0; i < 5; i++)
	{
		printf("%d\t\t%d\t\t%lf\n",
				products[i].product_ID,
				products[i].initial_quantity,
				products[i].product_Price);
	}
	
	
}
// prints updated product informations 
void print_updated_products()
{

	printf("Updated Products:\nProduct ID\tQuantity\tPrice\t\n");
	int i;
	for(i=0; i < 5; i++)
	{
		printf("%d\t\t%d\t\t%lf\n",
				products[i].product_ID,
				products[i].product_Quantity,
				products[i].product_Price);
	}	
}


// order process 
void order_product(struct Customer* customer,  struct Ordered_Product* ordered_item, struct Product* product)
{
	// mutex lock to not let work more than one process simultaneously
	sem_wait(sem_lock);
	// placeholders to make code simpler
	int pid, quantity;
	double total_price;
	int purchase_cnt = 0;
	struct Ordered_Product* purchased_item;

	// product id of ordered product
	pid = ordered_item->product_ID;
	// quantity of ordered product
	quantity = ordered_item->product_quantity;
	// calculated price by multiplying product price and quantity
	total_price = quantity * product->product_Price;

	// if not enough required quantity left of ordered product
	if(quantity > product->product_Quantity)
		printf("Customer%d(%d, %d) fail! Only %d left in stock.\n",
					customer->customer_ID, pid, quantity, product->product_Quantity);
	// if customer balance is not enough
	else if(customer->customer_Balance < total_price)
		printf("Customer%d(%d, %d) fail! Insufficient funds.\n",
					customer->customer_ID, pid, quantity);
	// if customer balance is enough and there is enough product in stock
	else
	{
		// print initial information
		print_initial_products(customer->customer_ID);
		// withdraw the price from the customer balance
		customer->customer_Balance -= total_price;

		// move until last allocated node
		purchased_item = customer->purchased_items;
		while(purchased_item->product_ID != 0)
			purchased_item = purchased_item->next;

		// assign attributes of the purchased item
		purchased_item->product_ID = pid;
		purchased_item->product_quantity = quantity;
		// allocate new space for next product
		purchased_item->next = malloc(sizeof(struct Ordered_Product));
		printf("Bought %d of product %d for $%lf\n",
				 quantity, pid, total_price);
		// update product quantities
		product->product_Quantity -= quantity;
		product->initial_quantity -= quantity;
		// print updated information
		print_updated_products();
		

	}
	// unlock to let other processes to continue
	sem_post(sem_lock);
	
}

// initialize product information
void initialize_products()
{
	// Initial ID's
	int product_id = 1;

	// Creatinng product instances
	// Product 1
	products[0].product_ID = product_id++;
	products[0].product_Price = 99.36;
	products[0].product_Quantity = 9;
	products[0].initial_quantity = 9;

	// Product 2
	products[1].product_ID = product_id++;
	products[1].product_Price = 78.77;
	products[1].product_Quantity = 5;
	products[1].initial_quantity = 5;

	// Product 3
	products[2].product_ID = product_id++;
	products[2].product_Price = 42.93;
	products[2].product_Quantity = 5;
	products[2].initial_quantity = 5;

	// Product 4
	products[3].product_ID = product_id++;
	products[3].product_Price = 14.40;
	products[3].product_Quantity = 3;
	products[3].initial_quantity = 3;

	// Product 5
	products[4].product_ID = product_id++;
	products[4].product_Price = 18.91;
	products[4].product_Quantity = 1;
	products[4].initial_quantity = 1;

}

// initialize customer informatiion
void initialize_customers()
{
	// placeholders to initalize customers
	// customer id
	int customer_id = 1;
	// items length that will be purchased by the customer
	int num_items = 2;
	// ordered product id
	int ordered_pid = 3;
	// ordered quantity
	int quantity = 4;
	// ordered item
	struct Ordered_Product* ordered_item;
	
	// Customer 1
	customers[0].customer_ID = customer_id++;
	customers[0].customer_Balance = 180;
	customers[0].initial_balance = 180;
	// allocate place for new possible products
	customers[0].ordered_items = malloc(sizeof(struct Ordered_Product)); 
	customers[0].purchased_items = malloc(sizeof(struct Ordered_Product));
	ordered_item = customers[0].ordered_items;
	// assign first ordered item information
	ordered_item->product_ID = ordered_pid;
    ordered_item->product_quantity = quantity;
	// allocate space for following node
    ordered_item->next = malloc(sizeof(struct Ordered_Product));
	// move to next node
    ordered_item = ordered_item->next;

	// initalize next ordered item
	ordered_pid = 4;
	quantity = 1;
	ordered_item->product_ID = ordered_pid;
    ordered_item->product_quantity = quantity;
	// since there will be no new ordered item by this customer NULL is assigned to next
    ordered_item->next = NULL;
	
	// Customer 2
	customers[1].customer_ID = customer_id++;
	customers[1].customer_Balance = 150;
	customers[1].initial_balance = 150;
	// allocate space for following node
	customers[1].ordered_items = malloc(sizeof(struct Ordered_Product)); 
	customers[1].purchased_items = malloc(sizeof(struct Ordered_Product));
	ordered_item = customers[1].ordered_items;

	ordered_pid = 3;
	quantity = 2;

	ordered_item->product_ID = ordered_pid;
    ordered_item->product_quantity = quantity;
	// allocate space for following node
    ordered_item->next = malloc(sizeof(struct Ordered_Product));
	// move to next node
    ordered_item = ordered_item->next;

	ordered_pid = 5;
	quantity = 1;
	ordered_item->product_ID = ordered_pid;
    ordered_item->product_quantity = quantity;
	// since there will be no new ordered item by this customer NULL is assigned to next
    ordered_item->next = NULL;

	// Customer 3
	customers[2].customer_ID = customer_id++;
	customers[2].customer_Balance = 130;
	customers[2].initial_balance = 130;
	customers[2].ordered_items = malloc(sizeof(struct Ordered_Product)); 
	customers[2].purchased_items = malloc(sizeof(struct Ordered_Product));
	ordered_item = customers[2].ordered_items;

	ordered_pid = 1;
	quantity = 1;
	ordered_item->product_ID = ordered_pid;
    ordered_item->product_quantity = quantity;
    ordered_item->next = malloc(sizeof(struct Ordered_Product));
	// allocate space for following node
    ordered_item = ordered_item->next;

	ordered_pid = 4;
	quantity = 1;
	ordered_item->product_ID = ordered_pid;
    ordered_item->product_quantity = quantity;
    ordered_item->next = malloc(sizeof(struct Ordered_Product));
	// allocate space for following node
    ordered_item = ordered_item->next;

	ordered_pid = 2;
	quantity = 1;
	ordered_item->product_ID = ordered_pid;
    ordered_item->product_quantity = quantity;
	// since there will be no new ordered item by this customer NULL is assigned to next
    ordered_item->next = NULL;
}

// process purchase for each customer
void process_customer(struct Customer* customer)
{
	struct Ordered_Product* ordered_item = customer->ordered_items;
	// check the lock and if unlocked lock and begin process

	// until all ordered items are processed
	while(ordered_item != NULL)
	{
		order_product(customer, ordered_item, &products[ordered_item->product_ID -1]);
		ordered_item = ordered_item->next;
		fflush(stdout);

	}


}

// print customer information
void print_customer(struct Customer* customer)
{
	// prints customer id, iniial balance of the customer and final balance
	printf("\nCustomer%d---------\ninitial balance: $%lf\nupdated balance: $%lf\nOrdered products:\nid\tquantity\n",
	customer->customer_ID, customer->initial_balance, customer->customer_Balance);
	// placeholder of ordered item
	struct Ordered_Product* ordered_item = customer->ordered_items;
	// until ordered item is not null print item information
	while(ordered_item != NULL)
	{
		printf("%d\t%d\n", ordered_item->product_ID, ordered_item->product_quantity);
		ordered_item = ordered_item->next;
	}
	// print purchased products
	printf("\nPurchased products:\nid\tquantity\n");
	// placeholder of purchased item
	struct Ordered_Product* purchased_item = customer->purchased_items;
	// print "-" if no product has been purchased by the customer
	if(purchased_item->product_ID == 0)
		printf("-\t-\n");
	else
	{	
		// print until product id is 0
		while(purchased_item->product_ID != 0)
		{
			printf("%d\t%d\n", purchased_item->product_ID, purchased_item->product_quantity);
			purchased_item = purchased_item->next;
		}
	}
	printf("\n...\n\n");
	fflush(stdout);
}

int main()
{
	
	// init mutex lock to work simultaneously 
    sem_lock = sem_open("/sem_lock", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	// initialize shared counter to check all process is finished or not
	// it will be used make all customer prints at the end
	int *shared_counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared_counter = 0;
	// make products to be shared to modify all information at all processes
	products = mmap(NULL, sizeof(struct Product) * NUM_PRODUCTS, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	
	initialize_products();
	initialize_customers();
	
	struct Ordered_Product* ordered_item;
    struct Ordered_Product* purchased_item;
	int status;
	int customer_id;
	// start and end time
	struct timeval start_time;
	struct timeval end_time;
	// placeholder of time elapsed
	long microseconds, seconds; 
	gettimeofday(&start_time, NULL);

	// pid to check parent or child
	// fork operation for multiprocessing
	pid_t pid;

	pid = fork();
	if(pid == 0)
		customer_id = 1;
	else
	{
		pid = fork();
		if(pid==0)
			customer_id = 2;
			
		else
			customer_id = 3;
		
	}
	// shopping process for each customer
	process_customer(&customers[customer_id-1]);

	// lock to not let continue other process at same time
	sem_wait(sem_lock);
    (*shared_counter)++;
	sem_post(sem_lock);

	// wait until all shopping processes are completed
	while (*shared_counter < NUM_CUSTOMERS)
		usleep(1000);
	// lock to not let continue other process at same time
	sem_wait(sem_lock);
	// print initial and final information
	print_customer(&customers[customer_id-1]);
	sem_post(sem_lock);
	
	// Destroy the named semaphore when done
    if (customer_id == 3)
    {
		// release the link
        sem_unlink("/sem_lock");
		// get time
		gettimeofday(&end_time, NULL);
		// seconds and microseconds to show total time that program consumed
		seconds = (end_time.tv_sec - start_time.tv_sec);
		microseconds = ((seconds * 1000000) + end_time.tv_usec) - (start_time.tv_usec);

		printf("Total time: %ld.%06ld seconds\n", seconds, microseconds);
		while (wait(&status) > 0);
    }


	return 0;
}

#include <stdio.h>
#include <stdlib.h>

// including months, contribution, and rate of returns in a struct named _retire_info
struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct _retire_info retire_info;

// including age and balance in a struct named _saving_info
struct _saving_info {
  int age;
  double balance;
};
typedef struct _saving_info saving_info;

// computing saving_info
saving_info compute_saving_info(saving_info age_balance, retire_info working) {
  printf("Age %3d month %2d you have $%.2lf\n",
         age_balance.age / 12,
         age_balance.age % 12,
         age_balance.balance);
  // update age
  age_balance.age += 1;
  // upgate balance
  age_balance.balance =
      age_balance.balance * (1 + working.rate_of_return / 12) + working.contribution;
  // print age and balance
  return age_balance;
}

// compute retirement account balance of working and retirement
void retirement(int startAge, double initial, retire_info working, retire_info retired) {
  saving_info age_balance;
  age_balance.age = startAge;
  age_balance.balance = initial;
  //  printf("Age %3d month %2d you have $%.2lf\n",
  //     age_balance.age / 12,
  //     age_balance.age % 12,
  //     age_balance.balance);
  for (int i = 1; i <= working.months; i++) {
    age_balance = compute_saving_info(age_balance, working);
  }
  for (int i = 1; i <= retired.months; i++) {
    age_balance = compute_saving_info(age_balance, retired);
  }
}

int main(void) {
  int startAge = 327;
  int initial = 21345;

  retire_info working;
  working.months = 489;
  working.contribution = 1000;
  working.rate_of_return = 0.045;

  retire_info retired;
  retired.months = 384;
  retired.contribution = -4000;
  retired.rate_of_return = 0.01;

  retirement(startAge, initial, working, retired);

  return EXIT_SUCCESS;
}

# About this example

I am implementing a setup where two processes: a producer and a consumer, share a single struct in shared memory.

Additionally, I use a semaphore set containing two semaphores to synchronize the two processes:

- `sem[0]` is used to wake up the consumer.
- `sem[1]` is used to wake up the producer.

The process is briefly described as follows:

- The producer continuously increments a `money` variable by 10 every second. When the value reaches 100, it wakes up the consumer via `sem[0]` and waits for a wake-up signal from `sem[1]`.
- The consumer waits for the wake-up signal from `sem[0]`, then subtracts 50 from the `money` variable; once this is done, it wakes up the producer via `sem[1]`.

The result is illustrated in the image below:

![alt text](image.png)

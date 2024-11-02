**Overview:** This model is used to compute Swap rate for a plain vanilla interest rate swap. The model takes SOFR rate curve as inputs and produces SWAP rate through boot strapping. The SOFR rate helps to estimate the cost of collateralized borrowing in the U.S Repo market. It is an overnight borrowing rate. Let us say the 6M SOFR rate is 4% and a bank wants to borrow $100 million, then it means it costs $2 million ($4million/2) to borrow $100 million through the repo. The swap rate helps to identify the cost of the hedging. It is the interest rate on the fixed payment leg of an interest rate swap. Let us say, 1 year swap rate is 4.5% and a bank wants to hedge $100 million variable loan exposure. Then the bank would pay $4.5 million to hedge this variable rate loan. The bank pays $2.25 million (4.5%/2) for every six months and receives 3M SOFR rate per quarters.  


**References:
**

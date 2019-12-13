# Project 2

**Lei Yang 903532922**

## Data

The small universe data is collected on `Quandl`. Missing data is filled forwardly. Data is normalized by ticker using formula $z = \frac{x-min(x)}{max(x)-min(x)}$. 

## Features

The features selected include:
+ Volume 
+ Close
+ MA5
+ MACD
+ MA20. Simple moving average.
+ Bollinger Bands
+ STD20. 20 days standard deviation.
+ RSI. Relative Strength Index

These features in theory should be preditive about the future stock prices as traders are actively using the indicators.

## Model

### Logistics Model

As the explanatory variable being `0-1` variable and main task is to classify the future trend into up and down, so using logistics model is intuitive to solve this kind of classfication issues. The hyperparamter is the regularization strength, smaller value indicates stronger regularization. 

### Random Forest

`Random Forest` is also a good classifier and is used in the industry, so I choose it as the second model. The hyperparamters space are

```json
{
    'n_estimators': [200, 400],
    'max_depth': [10, 15, 20],
    'min_samples_leaf': [50, 100]
}
```


## Conclusion

The accuracy is as expected not very high, as the features are only the results of transformation of the former stock prices. The small universe auc is about **51%**, for the large universe, the best auc could be as higher as **63%**, but it clearly has problems of over fitting. As for the model fitting, the random forest used here is slightly better than logistics model in terms of auc.

## Additional Considerations

The model does not have a high accuracy, even it does, it is still not enough to start trading on it. Because these models only predict the future movements, while for trading we need to know the direction, quantities of a trade.

If seeing only the best 20 results in the large universe, it could have overfitting problems. Overall, the influence of overfitting is small as the overall fitting results are not good enough.



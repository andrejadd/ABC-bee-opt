function X=X_random(n, x_max, y_max, x_min, y_min)
    X=[x_min+((rand(n,1)).*(x_max-x_min)), y_min+((rand(n,1)).*(y_max-y_min))];
end
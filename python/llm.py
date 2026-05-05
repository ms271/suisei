import pandas as pd
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression

csv_filename = 'profiler_data.csv' 

try:
    df = pd.read_csv(csv_filename)
except FileNotFoundError:
    print(f"Error: Could not find '{csv_filename}'. Make sure your C++ engine has generated it.")
    exit()

X = df[['Vertices', 'Lights', 'BaseProfile']].values
y = df['RenderTime'].values

num_samples = len(df)
print(f"Successfully loaded {num_samples} frames of profiling data.")

poly = PolynomialFeatures(degree=2, include_bias=False)
X_poly = poly.fit_transform(X)

model = LinearRegression()
model.fit(X_poly, y)

all_weights = [model.intercept_] + list(model.coef_)

weights_df = pd.DataFrame([all_weights])
weights_df.to_csv("model_weights.csv", index=False, header=False)

print("\n--- MODEL EXPORT COMPLETE ---")
print("Saved to 'model_weights.csv'. File contents:")

with open("model_weights.csv", "r") as f:
    print(f.read().strip())
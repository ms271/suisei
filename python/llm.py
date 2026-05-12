import pandas as pd
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression

csv_filename = 'python/profiler_data.csv' 

try:
    df = pd.read_csv(csv_filename)
except FileNotFoundError:
    print(f"Error: Could not find '{csv_filename}'. Make sure your C++ engine has generated it.")
    exit()

X = df[['Vertices']].values
y = df['RenderTime'].values

num_samples = len(df)
print(f"Successfully loaded {num_samples} frames of profiling data.")

poly = PolynomialFeatures(degree=1, include_bias=False)
X_poly = poly.fit_transform(X)

model = LinearRegression()
model.fit(X_poly, y)

all_weights = [model.intercept_] + list(model.coef_)

weights_df = pd.DataFrame([all_weights])
weights_df.to_csv("python/model_weights.csv", index=False, header=False)

print("\n--- MODEL EXPORT COMPLETE ---")
print("Saved to 'model_weights.csv'. File contents:")

with open("python/model_weights.csv", "r") as f:
    print(f.read().strip())

import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('python/profiler_data.csv')

plt.scatter(df['Vertices'], df['RenderTime'], s=5, alpha=0.5)
plt.xlabel('Vertices')
plt.ylabel('Render Time (ms)')
plt.title('Vertices vs Render Time')
plt.show()
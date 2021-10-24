from scipy.io.arff import loadarff
import pandas as pd
import numpy as np
from sklearn import datasets
import plotly.express as px
from sklearn.model_selection import train_test_split
from sklearn import neighbors
from sklearn import tree
from sklearn.metrics import accuracy_score


arff_data = loadarff('breast.w.arff')
df = pd.DataFrame(arff_data[0])
df['Class'] = pd.factorize(df['Class'])[0] + 0.0

x = np.array(df.iloc[:, :-1].values.tolist())   # data
y = df['Class'].to_numpy()                      # targets

x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=.5)

classifier = neighbors.KNeighborsClassifier()
classifier.fit(x_train, y_train)
predictions = classifier.predict(x_test)
print(accuracy_score(y_test, predictions))


""""
attributes = ["Clump_Thickness", "Cell_Size_Uniformity", "Cell_Shape_Uniformity",
              "Marginal_Adhesion", "Single_Epi_Cell_Size", "Bare_Nuclei",
              "Bland_Chromatin", "Normal_Nucleoli", "Mitoses"]
              
for attribute in attributes:
    fig_CP = px.histogram(df,
                          x=attribute,
                          color="Class",
                          labels={attribute: attribute.replace('_', ' ')})
    fig_CP.show()

fig = px.scatter_matrix(df,
                        dimensions=attributes,
                        color="Class",
                        symbol="Class",
                        labels={col: col.replace('_', ' ') for col in df.columns})
fig.update_traces(diagonal_visible=False)
fig.show()
"""
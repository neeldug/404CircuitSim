import plotly.express as px
import pandas as pd
import argparse

parser = argparse.ArgumentParser(description="Add Options to Control Plot")
parser.add_argument("file_name", metavar="FILE" ,type=str, nargs=1, help="File name for SPICE output to plot (.csv formatting)")
parser.add_argument("include", metavar="COLUMN_NAME", type=str, nargs='*', help="Space separated list of columns to plot, if omitted all columns will be plotted", default=None)

def plot(file_name, include=None):
    df = pd.read_csv(file_name).dropna(axis=1, how='all')
    column_names = df.columns.values if not include else ["Time", *include]
    df_vals = pd.melt(df, id_vars=column_names[0], value_vars=column_names[1:])
    fig = px.line(df_vals, x="Time", y="value", color='variable')
    fig.show()

if __name__ == '__main__':
    args = parser.parse_args()
    print(args.include)
    plot(args.file_name[0], args.include)
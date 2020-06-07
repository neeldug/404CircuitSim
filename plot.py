import plotly.express as px
import pandas as pd
import argparse
import plotly.graph_objs as go
import os

parser = argparse.ArgumentParser(description="Add Options to Control Plot")
parser.add_argument("file_name", metavar="FILE", type=str, nargs=1,
                    help="Path of file to plot")
parser.add_argument("-c", "--column-names",
                    action='store_true', help="show column names")
parser.add_argument("-m", "--mode", type=str, nargs=1,
                    help="Either space or csv formatted documents can be plotted, default is csv")
parser.add_argument("include", metavar="COLUMN_NAME", type=str, nargs='*',
                    help="Space separated list of columns to plot, if omitted all columns will be plotted", default=None)


if __name__ == '__main__':
    args = parser.parse_args()
    sep = ',' if args.mode is None or args.mode[0].lower() == 'csv' else '\t'

    df = pd.read_csv(args.file_name[0], sep=sep,
                     engine='python').dropna(axis=1, how='all')

    if args.column_names:
        print(df.columns.values[1:])
        exit(0)
    else:
        column_names = df.columns.values if not args.include else [
            "Time", *args.include]
        df_vals = pd.melt(
            df, id_vars=column_names[0], value_vars=column_names[1:])
        # fig = px.line(df_vals, x="Time", y="value", color='variable')
        fig = go.Figure(data=px.line(df_vals, x="Time",
                                     y="value", color='variable'))
        fig.update_layout(title_text="Scale",
                          updatemenus=[
                              dict(
                                  buttons=list([
                                      dict(label="X Linear",
                                           method="relayout",
                                           args=[{"xaxis.type": "linear"}]),
                                      dict(label="X Log",
                                           method="relayout",
                                           args=[{"xaxis.type": "log"}]),
                                      dict(label="Y Linear",
                                           method="relayout",
                                           args=[{"yaxis.type": "linear"}]),
                                      dict(label="Y Log",
                                           method="relayout",
                                           args=[{"yaxis.type": "log"}]),
                                  ]),
                              )])
        fig.show()

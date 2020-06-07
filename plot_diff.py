import plotly.express as px
import pandas as pd
import argparse
import plotly.graph_objs as go
from plotly.subplots import make_subplots

parser = argparse.ArgumentParser(description="Add Options to Control Plot")
parser.add_argument("file_name", metavar="GENERATED_FILE.csv SPICE_FILE", type=str, nargs=2,
                    help="The path to the files to compare")


if __name__ == '__main__':
    args = parser.parse_args()
    generated = pd.read_csv(args.file_name[0]).dropna(axis=1, how='all')
    spice = pd.read_csv(args.file_name[1], sep='\t').dropna(axis=1, how='all')

    fig = make_subplots(rows=len(generated.columns)-2, cols=1, subplot_titles=generated.columns.values[2:])

    for i, gen_col in enumerate(generated.columns.values[2:]):
        for spice_col in spice.columns.values[1:]:
            if gen_col.lower() == spice_col.lower():
                gen_sub = go.Scatter(x = generated['Time'], y=generated[gen_col],
                            line=dict(color='firebrick'), name=gen_col)
                
                spice_sub = go.Scatter(x = spice['time'], y=spice[spice_col], 
                            line=dict(color='royalblue', dash='dash'), name=spice_col)
                
                fig.add_trace(gen_sub, row=i+1, col=1)
                fig.add_trace(spice_sub, row=i+1, col=1)

                fig.update_xaxes(title_text='Time/s', row=i+1, col=1)
                y_title = 'Voltage/V' if gen_col[0] == 'V' else 'Current/A'
                fig.update_yaxes(title_text=y_title, row=i+1, col=1)

    
    fig.show()
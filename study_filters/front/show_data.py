import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import interpolate
import os

# Настройка красивого стиля графиков
plt.style.use('seaborn-v0_8-whitegrid')

# Чтение данных из файла (укажите путь к вашему файлу)
file_path = "/home/butterfly/Templates/sol_celeron/study_filters/back/out.txt"  # ЗАМЕНИТЕ на ваш путь

try:
    # Чтение данных (предполагается, что первая колонка - x, остальные - y1, y2, y3)
    data = pd.read_csv(file_path, sep='\t', header=None, names=['x', 'y1', 'y2', 'y3'])
    
    # Создание фигуры и осей
    fig, ax = plt.subplots(figsize=(12, 8))
    
    # Цвета для графиков
    colors = ['#FF6B6B', '#4ECDC4', '#45B7D1']
    labels = ['y1', 'y2', 'y3']
    
    # Построение плавных графиков
    for i, (col, color, label) in enumerate(zip(['y1', 'y2', 'y3'], colors, labels)):
        x = data['x'].values
        y = data[col].values
        
        # Создание плавной интерполяции
        if len(x) > 3:  # Интерполяция требует минимум 4 точки
            x_smooth = np.linspace(x.min(), x.max(), 300)
            f = interpolate.interp1d(x, y, kind='cubic', fill_value='extrapolate')
            y_smooth = f(x_smooth)
            
            # Построение графика
            ax.plot(x_smooth, y_smooth, color=color, linewidth=2.5, label=label, alpha=0.8)
            ax.scatter(x, y, color=color, s=30, alpha=0.6)  # Исходные точки
        else:
            ax.plot(x, y, color=color, linewidth=2.5, label=label, marker='o')
    
    # Настройка внешнего вида
    ax.set_xlabel('X', fontsize=12, fontweight='bold')
    ax.set_ylabel('Y', fontsize=12, fontweight='bold')
    ax.set_title('Плавные графики данных', fontsize=14, fontweight='bold')
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3)
    
    # Красивые оси
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    
    plt.tight_layout()
    plt.show()
    
except FileNotFoundError:
    print(f"Файл не найден: {file_path}")
except Exception as e:
    print(f"Ошибка при обработке файла: {e}")
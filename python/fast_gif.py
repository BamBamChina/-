import numpy as np
import matplotlib.pyplot as plt
import imageio
import os

def create_fast_gif(csv_file, output_gif, mu, step=10, max_frames=200):
    """Быстрое создание GIF с помощью imageio"""
    
    print(f"Загрузка {csv_file}...")
    data = np.loadtxt(csv_file, delimiter=',', skiprows=1)
    
    if len(data) == 0:
        print("Файл пуст!")
        return
    
    t = data[:, 0]
    x = data[:, 1]
    y = data[:, 2]
    
    # Удаляем NaN
    mask = np.isfinite(x) & np.isfinite(y)
    x = x[mask]
    y = y[mask]
    t = t[mask]
    
    # Прореживаем
    x = x[::step]
    y = y[::step]
    t = t[::step]
    
    if len(x) > max_frames:
        step2 = len(x) // max_frames
        x = x[::step2]
        y = y[::step2]
        t = t[::step2]
    
    print(f"Кадров: {len(x)}")
    
    L4_x = 0.5 - mu
    L4_y = np.sqrt(3) / 2
    m1_x = -mu
    m2_x = 1 - mu
    
    # Определяем границы
    x_max = max(1.5, np.percentile(np.abs(x), 95) * 1.2)
    y_max = max(1.5, np.percentile(np.abs(y), 95) * 1.2)
    
    frames = []
    
    for i in range(len(x)):
        if i % 20 == 0:
            print(f"  Кадр {i}/{len(x)}")
        
        fig, ax = plt.subplots(figsize=(8, 8))
        ax.set_xlim(-x_max, x_max)
        ax.set_ylim(-y_max, y_max)
        ax.set_aspect('equal')
        ax.grid(True, alpha=0.3)
        
        # Статические объекты
        ax.plot(m1_x, 0, 'ko', markersize=15)
        ax.plot(m2_x, 0, 'ko', markersize=10)
        ax.plot(L4_x, L4_y, 'r*', markersize=15)
        
        # Траектория до текущего момента
        ax.plot(x[:i+1], y[:i+1], 'b-', lw=0.8, alpha=0.6)
        ax.plot(x[i], y[i], 'bo', markersize=4)
        ax.set_title(f'L₄ при μ = {mu:.6f}, t = {t[i]:.1f}')
        
        # Сохраняем кадр в память (ИСПРАВЛЕННАЯ СТРОКА)
        fig.canvas.draw()
        
        # Для новых версий matplotlib (>=3.8)
        try:
            # Пробуем новый способ
            buf = fig.canvas.buffer_rgba()
            frame = np.asarray(buf)
        except AttributeError:
            # Для старых версий
            frame = np.frombuffer(fig.canvas.tostring_rgb(), dtype='uint8')
            frame = frame.reshape(fig.canvas.get_width_height()[::-1] + (3,))
        
        frames.append(frame)
        plt.close(fig)
    
    print(f"Сохраняем {output_gif}...")
    imageio.mimsave(output_gif, frames, fps=30)
    print(f"✅ Готово!")


# Ещё более простая версия, которая точно должна работать
def create_simple_gif(csv_file, output_gif, mu, max_frames=200):
    """Самая простая версия — сохраняет кадры как PNG, потом собирает GIF"""
    
    import tempfile
    import shutil
    
    print(f"Загрузка {csv_file}...")
    data = np.loadtxt(csv_file, delimiter=',', skiprows=1)
    
    if len(data) == 0:
        print("Файл пуст!")
        return
    
    t = data[:, 0]
    x = data[:, 1]
    y = data[:, 2]
    
    mask = np.isfinite(x) & np.isfinite(y)
    x = x[mask]
    y = y[mask]
    t = t[mask]
    
    # Прореживаем
    step = max(1, len(x) // max_frames)
    x = x[::step]
    y = y[::step]
    t = t[::step]
    
    print(f"Кадров: {len(x)}")
    
    L4_x = 0.5 - mu
    L4_y = np.sqrt(3) / 2
    m1_x = -mu
    m2_x = 1 - mu
    
    x_max = max(1.5, np.percentile(np.abs(x), 95) * 1.2)
    y_max = max(1.5, np.percentile(np.abs(y), 95) * 1.2)
    
    # Создаём временную папку для кадров
    temp_dir = tempfile.mkdtemp()
    
    try:
        for i in range(len(x)):
            if i % 20 == 0:
                print(f"  Кадр {i}/{len(x)}")
            
            fig, ax = plt.subplots(figsize=(8, 8))
            ax.set_xlim(-x_max, x_max)
            ax.set_ylim(-y_max, y_max)
            ax.set_aspect('equal')
            ax.grid(True, alpha=0.3)
            
            ax.plot(m1_x, 0, 'ko', markersize=15)
            ax.plot(m2_x, 0, 'ko', markersize=10)
            ax.plot(L4_x, L4_y, 'r*', markersize=15)
            ax.plot(x[:i+1], y[:i+1], 'b-', lw=0.8, alpha=0.6)
            ax.plot(x[i], y[i], 'bo', markersize=4)
            ax.set_title(f'μ = {mu:.6f}, t = {t[i]:.1f}')
            
            # Сохраняем как PNG
            frame_path = os.path.join(temp_dir, f"frame_{i:04d}.png")
            plt.savefig(frame_path, dpi=80, bbox_inches='tight')
            plt.close(fig)
        
        # Собираем PNG в GIF
        print(f"Собираем GIF из PNG...")
        import glob
        png_files = sorted(glob.glob(os.path.join(temp_dir, "frame_*.png")))
        
        # Читаем все PNG в frames
        frames = []
        for png_file in png_files:
            from PIL import Image
            frames.append(Image.open(png_file))
        
        # Сохраняем как GIF
        frames[0].save(output_gif, save_all=True, append_images=frames[1:], 
                      duration=33, loop=0)  # duration в миллисекундах (33ms = 30 fps)
        print(f"✅ Готово: {output_gif}")
        
    finally:
        # Удаляем временную папку
        shutil.rmtree(temp_dir, ignore_errors=True)


if __name__ == "__main__":
    print("=" * 50)
    print("СОЗДАНИЕ GIF АНИМАЦИЙ")
    print("=" * 50)
    
    mu_values = [0.01, 0.0242938, 0.013516]
    
    for mu in mu_values:
        csv_file = f"trajectory_mu_{mu}.csv"
        if os.path.exists(csv_file):
            print(f"\nОбработка μ = {mu}")
            # Используем простую версию (работает гарантированно)
            create_simple_gif(csv_file, f"L4_mu_{mu}.gif", mu, max_frames=200)
        else:
            print(f"\nФайл {csv_file} не найден!")
    
    print("\n" + "=" * 50)
    print("ГОТОВО!")

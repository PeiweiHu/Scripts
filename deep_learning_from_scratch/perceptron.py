class Perceptron:
    def __init__(self, dim, activate_func):
        """ 
        初始化输入维度、激活函数、学习率、权重和偏置项
        """
        self.dim = dim
        self.activate_func = activate_func
        self.learn_rate = 0.05
        self.weight = [0.0] * self.dim
        self.bias = 0.0
    
    def __str__(self):
        return 'weight: {0} bias: {1}'.format(str(self.weight), str(self.bias))
    
    def predict(self, input_vector):
        zipped = list(zip(self.weight, input_vector))
        res = sum(list(map(lambda x: x[0]*x[1], zipped))) + self.bias
        return self.activate_func(res)

    def update_weight(self, input_vector, label):
        predict_val = self.predict(input_vector)
        delta = label - predict_val
        # update weight
        self.weight = list(map(lambda x: x[0]+x[1]*delta*self.learn_rate, zip(self.weight, input_vector)))
        # update bias
        self.bias = self.bias + delta*self.learn_rate
    
    def one_round(self, input_vector_lst, label_lst):
        for input_vector, label in zip(input_vector_lst, label_lst):
            self.update_weight(input_vector, label)

    def train(self, input_vector_lst, label_lst, round):
        for _r in range(round):
            print(self)
            self.one_round(input_vector_lst, label_lst)


if __name__ == '__main__':
    # 训练执行or操作的感知器
    input_vector_lst = [ [1,1], [1,0], [0,1], [0,0] ]
    label_lst = [1, 1, 1, 0]
    p = Perceptron(2, lambda x: 1 if x > 0 else 0)
    p.train(input_vector_lst, label_lst, 10)
    print('predict: ')
    print(p.predict([0,0]))
    print(p.predict([0,1]))
    print(p.predict([1,0]))
    print(p.predict([1,1]))
    # 训练执行and操作的感知器
    input_vector_lst = [ [1,1], [1,0], [0,1], [0,0] ]
    label_lst = [1, 0, 0, 0]
    p = Perceptron(2, lambda x: 1 if x > 0 else 0)
    p.train(input_vector_lst, label_lst, 10)
    print('predict: ')
    print(p.predict([0,0]))
    print(p.predict([0,1]))
    print(p.predict([1,0]))
    print(p.predict([1,1]))

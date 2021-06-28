class LinearUnit:
    def __init__(self, dim):
        """ 
        初始化输入维度、激活函数、学习率、权重和偏置项
        """
        self.dim = dim
        self.activate_func = lambda n: n
        self.learn_rate = 0.05
        self.weight = [0.0] * self.dim
        self.bias = 0.0
    
    def __str__(self):
        return 'weight: {0} bias: {1}'.format(str(self.weight), str(self.bias))
    
    def predict(self, input_vector):
        zipped = list(zip(self.weight, input_vector))
        res = sum(list(map(lambda x: x[0]*x[1], zipped))) + self.bias
        return self.activate_func(res)

    def batch_gradient_descent(self, input_vector_lst, label_lst):
        x_sum = [0.0] * self.dim
        bias_sum = 0
        for input_vector, label in zip(input_vector_lst, label_lst):
            y = self.predict(input_vector)
            delta = label - y
            bias_sum += delta
            x_delta = list(map(lambda n: delta * n, input_vector))
            x_sum = list(map(lambda n: n[0] + n[1], zip(x_sum, x_delta)))
        self.weight = list(map(lambda n: n[0] + self.learn_rate * n[1], zip(self.weight, x_sum)))
        self.bias += self.learn_rate * bias_sum

    def stochastic_gradient_descent(self, input_vector_lst, label_lst):
        for input_vector, label in zip(input_vector_lst, label_lst):
            y = self.predict(input_vector)
            delta = label - y
            x_delta = list(map(lambda n: delta * n, input_vector))
            self.weight = list(map(lambda n: n[0] + self.learn_rate * n[1], zip(self.weight, x_delta)))
            self.bias += self.learn_rate * delta

    def bgd_train(self, input_vector_lst, label_lst, round):
        print(self)
        for _r in range(round):
            self.batch_gradient_descent(input_vector_lst, label_lst)
            print(self)

    def sgd_train(self, input_vector_lst, label_lst, round):
        print(self)
        for _r in range(round):
            self.stochastic_gradient_descent(input_vector_lst, label_lst)
            print(self)

# def plot(linear_unit):
#     import matplotlib.pyplot as plt
#     input_vecs, labels = get_training_dataset()
#     fig = plt.figure()
#     ax = fig.add_subplot(111)
#     ax.scatter(map(lambda x: x[0], input_vecs), labels)
#     weights = linear_unit.weights
#     bias = linear_unit.bias
#     x = range(0,12,1)
#     y = map(lambda x:weights[0] * x + bias, x)
#     ax.plot(x, y)
#     plt.show()


if __name__ == '__main__':
    # first exp 
    # model = LinearUnit(1)
    # input_vector_lst = [[5], [3], [8], [1.4], [10.1]]
    # label_lst = [5500, 2300, 7600, 1800, 11400]
    # model.bgd_train(input_vector_lst, label_lst, 80)
    # second exp
    input_vector_lst = []
    label_lst = []
    with open('housing.data', 'r') as f:
        for _line in f.readlines():
            _line = _line.strip().split()
            input_vector_lst.append([float(x) for x in _line[:13]])
            label_lst.append(float(_line[-1]))
    # for input_vec, label in zip(input_vector_lst, label_lst):
    #     print(input_vec, label)  
    model = LinearUnit(13)
    model.sgd_train(input_vector_lst, label_lst, 8)
    print('\n\n')
    print(model.predict(input_vector_lst[-1]))
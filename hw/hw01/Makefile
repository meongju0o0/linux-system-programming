# TARGET은 생성될 최종 실행 파일의 이름을 저장
TARGET = app

# SRC는 ./src 디렉토리 내의 모든 .cpp 소스 파일을 재귀적으로 찾아 저장
SRCS  = $(shell find ./src -type f -name *.cpp)

# HEADS는./include 디렉토리 내의 모든 .h 헤더 파일을 재귀적으로 찾아 저장
HEADS = $(shell find ./include -type f -name *.h)

# OBJ는 SRCS에 있는 각 .cpp 파일에 대응하는 오브젝트 파일(.o)를 생성
OBJS = $(SRCS:.cpp=.o)

# DEPS는 makefile의 의존성 정보를 저장할 파일 이름을 지정
DEPS = Makefile.depend

# INCLUDES는 컴파일 시 사용될 헤더 파일의 경로를 지정
# -I 옵션을 통해 헤더 파일을 찾을 경로를 추가로 지정
INCLUDES = -I./include

# CXXFLAGS는 C++ 소스 파일을 컴파일 할 때 컴파일러에게 전달할 컴파일 옵션을 지정
# -02: 컴파일러 최적화 옵션, 일반적인 최적화 수준으로 코드 실행 속도를 개선
# -Wall: 경고 메세지 활성화 옵션, 모든 일반적인 경고 메세지 활성화
# $(INCLUDES): 추가적인 헤더 파일 경로를 포함하기 위한 옵션
CXXFLAGS = -O2 -Wall $(INCLUDES)

# LDFLAGS는 링커가 사용할 플래그를 지정, 라이브러리 링크(예: 수학라이브러리 -lm을 포함)
LDFLAGS = -lm

# 'all'은 최종 실행 파일인 TARGET을 빌드하는 기본 TARGET
all: $(TARGET)

# TARGET을 빌드하기 위한 규칙. 이 규칙은 오브젝트 파일(OBJS)와 헤더 파일(HEADS)에 의존
# CXX 변수 (C++ 컴파일러)를 사용하여 오브젝트 파일을 링크하고 실행 파일을 생성
$(TARGET): $(OBJS) $(HEADS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

# 'run'은 프로그램을 비륻하고 실행하기 위한 편의 목표
# '@' 기호는 실행되는 명령어의 출력 표시를 억제
run: all
	@./$(TARGET)

# .PHONY는 'depend'와 'clean'이 실제 파일이 아닌 명령어를 위한 라벨임을 나타냄
.PHONY: depend clean

# 'depend'는 모든 소스 파일의 의존성 파일을 생성하기 위한 타겟
# 의존성 파일은 -MM 플래그를 사용하여 생성되며, DEPS에 저장
# 'sed' 명령어는 생성된 의존성 파일 형식을 makefile에 맞게 수정
depend:
	$(CXX) $(INCLUDES) -MM $(SRCS) > $(DEPS)
	@sed -i -E "s/^(.+?).o: ([^ ]+?)\1/\2\1.o: \2\1/g" $(DEPS)

# 'clean'은 오브젝트 파일과 실행 파일을 삭제하기 위한 타겟
# 지정된 파일을 삭제하기 위해 RM 변수(보통 'rm -f'로 매핑)를 사용
clean:
	$(RM) $(OBJS) $(TARGET)

# -include는 의존성 파일(DEPS)이 존재할 경우 포함
#  이를 통해 헤더 파일의 변경 사항을 추적하고 관련된 소스 파일을 재컴파일할 수 있음
-include $(DEPS)

#include "../includes/job.hpp"
#include <thread>

namespace GeoFrame {
namespace Utils {
ObjectID const JobBase::sOID = ObjectID("JOB_BASE");
ObjectID const SimpleJob::sOID = ObjectID("SIMPLE_JOB");

void JobBase::Run() {
  try {
    this->Execute();
  } catch (...) {
    mException = std::current_exception();
  }
  mFinished = true;
}

bool JobBase::IsExecutable() const {
  for (auto &dep : mDependencies) {
    if (!dep->IsFinished()) {
      return false;
    }
  }
  return true;
}

SimpleJob &SimpleJob::operator=(Function<void()> const &target) {
  mFunction = target;
  return *this;
}

void JobSystem::WorkerThread() {
  while (!mStop) {
    JobBase *job = nullptr;
    {
      std::unique_lock<std::mutex> lock(mJobLock);
      mCondition.wait(lock, [this] { return !mJobs.empty() || mStop; });

      if (mJobs.empty()) {
        return;
      }

      while (true) {
        job = std::move(mJobs.front());
        mJobs.pop();

        if (job->IsExecutable()) {
          break;
        } else {
          mJobs.push(job);
        }
      }
    }

    job->Run();

    if (mNumJobs.fetch_sub(1) == 1) {
      mComplete.store(true);
      mComplete.notify_all();
    }
  }
}

void JobSystem::DaemonThread(JobBase *job) {
  while (!mStop) {
    job->Run();
  }
}

JobSystem::JobSystem(unsigned const &numThreads) {
  for (unsigned i = 0; i < numThreads; ++i) {
    mWorkers.emplace_back(Thread([this] { this->WorkerThread(); }));
  }
}

void JobSystem::Stop() {
  mStop.store(true);
  mCondition.notify_all();

  for (auto &worker : mWorkers) {
    worker.join();
  }

  for (auto &daemon : mDaemons) {
    daemon.join();
  }
}

void JobSystem::Schedule(JobBase *job) {
  {
    std::unique_lock<std::mutex> lock(mJobLock);
    mJobs.push(job);
    mNumJobs.fetch_add(1);
    mComplete.store(false);
  }

  mCondition.notify_one();
}
} // namespace Utils
} // namespace GeoFrame

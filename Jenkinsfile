@Library('xmos_jenkins_shared_library@v0.20.0') _

getApproval()

pipeline {
    agent none
        options {
        disableConcurrentBuilds()
        skipDefaultCheckout()
        timestamps()
        // on develop discard builds after a certain number else keep forever
        buildDiscarder(logRotator(
            numToKeepStr:         env.BRANCH_NAME ==~ /develop/ ? '25' : '',
            artifactNumToKeepStr: env.BRANCH_NAME ==~ /develop/ ? '25' : ''
        ))
    }
    parameters {
        string(
            name: 'TOOLS_VERSION',
            defaultValue: '15.2.1',
            description: 'The XTC tools version'
        )
        booleanParam(
            name: 'NIGHTLY_TEST_ONLY',
            defaultValue: false,
            description: 'Tests that only run during nightly builds.'
        )
    }
    environment {
        PYTHON_VERSION = "3.8.11"
        VENV_DIRNAME = ".venv"
        BUILD_DIRNAME = "dist"
        VRD_TEST_RIG_TARGET = "xcore_voice_test_rig"
    }
    stages {
        stage('QSPI fast read tests') {
            agent {
                label 'vrd-us'
            }
            stages{
                stage('Checkout') {
                    steps {
                        checkout scm
                        sh 'git submodule update --init --recursive --depth 1 --jobs \$(nproc)'
                    }
                }
                stage('Create virtual environment') {
                    steps {
                        // Create venv
                        sh "pyenv install -s $PYTHON_VERSION"
                        sh "~/.pyenv/versions/$PYTHON_VERSION/bin/python -m venv $VENV_DIRNAME"
                        // Install dependencies
                        withVenv() {
                            sh "pip install git+https://github0.xmos.com/xmos-int/xtagctl.git"
                        }
                    }
                }
                stage('Cleanup xtagctl') {
                    steps {
                        // Cleanup any xtagctl cruft from previous failed runs
                        withTools(params.TOOLS_VERSION) {
                            withVenv {
                                sh "xtagctl reset_all $VRD_TEST_RIG_TARGET"
                            }
                        }
                        sh "rm -f ~/.xtag/status.lock ~/.xtag/acquired"
                    }
                }
                stage('Set up hardware') {
                    steps {
                        withTools(params.TOOLS_VERSION) {
                            withVenv {
                                script {
                                    withXTAG(["$VRD_TEST_RIG_TARGET"]) { adapterIDs ->
                                        sh "tools/ci/config_hardware_str_50.sh " + adapterIDs[0]
                                    }
                                }
                            }
                        }
                    }
                }
                stage('Build tests') {
                    steps {
                        withTools(params.TOOLS_VERSION) {
                            script {
                                sh "tools/ci/build_tests.sh"
                            }
                        }
                    }
                }
                stage('Run tests') {
                    steps {
                        withTools(params.TOOLS_VERSION) {
                            script {
                                withXTAG(["$VRD_TEST_RIG_TARGET"]) { adapterIDs ->
                                    sh "tools/ci/run_tests.sh"
                                }
                            }
                        }
                    }
                }
            }
            post {
                always {
                    withTools(params.TOOLS_VERSION) {
                        withVenv {
                            script {
                                withXTAG(["$VRD_TEST_RIG_TARGET"]) { adapterIDs ->
                                    sh "tools/ci/restore_factory_settings.sh " + adapterIDs[0]
                                }
                            }
                        }
                    }
                }
                cleanup {
                    // cleanWs removes all output and artifacts of the Jenkins pipeline
                    //   Comment out this post section to leave the workspace which can be useful for running items on the Jenkins agent. 
                    //   However, beware that this pipeline will not run if the workspace is not manually cleaned.
                    cleanWs()
                }
            }
        }
    }
}

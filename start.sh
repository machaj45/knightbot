#!/bin/bash
MY_PATH=~/kinect_workspace/src/openni_tracker/tmux_scripts
MY_PATH=`( cd "$MY_PATH" && pwd )`

# remove the old link
rm $MY_PATH/.tmuxinator.yml

# link the session file to .tmuxinator.yml
ln $MY_PATH/session.yml $MY_PATH/.tmuxinator.yml

# start tmuxinator
cd $MY_PATH
tmuxinator

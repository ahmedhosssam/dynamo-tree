import { useEffect, useState } from 'react'
import axios from 'axios'

const Account = ({activities, username}) => {
  const [isAddActivity, setIsAddActivity] = useState(false)
  const [isDequeue, setIsDequeue] = useState(true)
  const [selectedOption, setSelectOption] = useState('like')
  const [newActivityContent, setNewActivityContent] = useState('')
  const [accountActivities, setAccountActivities] = useState(activities)

  useEffect(() => {
    setAccountActivities(activities)
    if (accountActivities.length > 0) {
      setIsDequeue(true)
    }
  }, [activities])

  const handleSelectOption = (e) => {
    setSelectOption(e.target.value)
  }

  const handleNewActivityContentChange = (e) => {
    setNewActivityContent(e.target.value)
  }

  const handleActivitySubmit = (e) => {
    const newActivity = {
      activityContent: newActivityContent,
      activityType: selectedOption,
      activityID: Math.round(Math.random() * 999999),
      username: username,
      order: "addActivity"
    }
    axios.post("http://localhost:8080/", newActivity)
    const newActivities = accountActivities.concat([newActivity])
    setAccountActivities(newActivities)
    setNewActivityContent('')
    setIsAddActivity(false)
    setIsDequeue(true)
  }

  const handleDequeue = () => {
    const dequeueObj = {
      order: "dequeue",
      username: username
    }
    axios.post("http://localhost:8080/", dequeueObj)
    location.reload()
  }

  return (
    <div>
      {isDequeue && <button onClick={handleDequeue}>Dequeue</button> }
      {activities && accountActivities.map(activity => 
        <div key={activity.id}>
          <Activity
            id = {activity.id}
            activityContent = {activity.activityContent}
            activityType = {activity.activityType}
          />
          <br />
        </div>)}
        <button onClick={() => {setIsAddActivity(!isAddActivity)}}>Add Activity</button>

        {isAddActivity &&
        <form action="submit" onSubmit={handleActivitySubmit}>
          <label htmlFor="activity name">Activity Type: </label>
          <select value={selectedOption} onChange={handleSelectOption} name="type" id="type">
            <option value="like">Like</option>
            <option value="post">Post</option>
            <option value="comment">Comment</option>
            <option value="Share">Share</option>
          </select>
          <br />
          <label htmlFor="activity name">Activity Content: </label>
          <input value={newActivityContent} onChange={handleNewActivityContentChange} type="text" />
          <button type='submit'>Submit</button>
        </form>
        }
    </div>
  )
}

const Activity = ({
  activityType,
  id,
  activityContent,
}) => {
  const [isClicked, setIsClicked] = useState(false)

  const handleDetailsClick = () => {
    setIsClicked(!isClicked)
  }

  return (
    <div>
      <button onClick={handleDetailsClick}>➤</button> {activityContent}
      {isClicked && <p>Activity Type: {activityType}</p>}
      {isClicked && <p>ID: {id}</p>}
    </div>
  )
}

const CreateNewUser = ({accounts}) => {
  const [username, setUsername] = useState('')

  const handleChangeUsername = (e) => {
    setUsername(e.target.value)
  }

  const handleCreateSubmit = (e) => {
    if (username === '') {
      e.preventDefault()
      return
    }
    if (accounts.find(account => account.username === username)) {
      e.preventDefault()
      alert(`${username} is already used.`)
      return
    }
    const newUser = {username: username, id: Math.round(Math.random() * 999999), activities: []}
    axios.post("http://localhost:8080/", newUser)
    setUsername('')
  }

  return (
    <div>
      <form action="submit" onSubmit={handleCreateSubmit}>
        <label htmlFor="usename">User Name: </label>
        <input type="text" value={username} onChange={handleChangeUsername} />
        <button type='submit'>Save</button>
      </form>
    </div>
  )
}

const App = () => {
  const [accounts, setAccounts] = useState([{username: "", id: 9999999, activities: [""]}])
  const [selectedOption, setSelectedOption] = useState('ahmedhossam23')
  const [selectedAccount, setSelectedAccount] = useState({})
  const [isCreateUserClicked, setIsCreateUserClicked] = useState(false)

  useEffect(() => {
    axios.get("http://localhost:8080/").then(res => {
      const data = res.data
      setAccounts(data)
      setSelectedAccount(data[0])
    })
  }, [])

  const handleSelectOption = (e) => {
    const newSelectedAccount = accounts.find(account => account.username === e.target.value)
    setSelectedOption(e.target.value)
    setSelectedAccount(newSelectedAccount)
  }

  const handleDeleteUser = (e) => {
    console.log(selectedAccount.username)
    axios.post("http://localhost:8080/", {order: "delete", target: selectedAccount.username})
    setAccounts(accounts.filter(account => account.id !== selectedAccount.id ))
    setSelectedAccount(accounts[0])
    setSelectedOption(accounts[0].username)
  }

  return (
    <div>
      <header>Social Media Tracker</header>
      <div>
        <p>Select Account:</p>
        <select value={selectedOption} onChange={handleSelectOption}>
          {accounts.map(account => <option key={account.id} value={account.username}>{account.username}</option>)}
        </select>
      </div>
      <button onClick={() => {setIsCreateUserClicked(!isCreateUserClicked)}}>Create a new user</button>
      {isCreateUserClicked && <CreateNewUser accounts={accounts}/>}
      <p>{selectedOption}:</p>

      <div>
        {selectedAccount.activities && <Account username={selectedAccount.username} activities={selectedAccount.activities}/>}
      </div>
      <br />
      <button onClick={handleDeleteUser}>Delete User</button>
    </div>
  )
}

export default App

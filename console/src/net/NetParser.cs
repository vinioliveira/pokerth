﻿/***************************************************************************
 *   Copyright (C) 2008 by Lothar May                                      *
 *                                                                         *
 *   This file is part of pokerth_console.                                 *
 *   pokerth_console is free software: you can redistribute it and/or      *
 *   modify it under the terms of the GNU Affero General Public License    *
 *   as published by the Free Software Foundation, either version 3 of     *
 *   the License, or (at your option) any later version.                   *
 *                                                                         *
 *   pokerth_console is distributed in the hope that it will be useful,    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the                                *
 *   GNU Affero General Public License along with pokerth_console.         *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 ***************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;

namespace pokerth_console
{
	class NetParser : INetPacketVisitor
	{
		public NetParser(PokerTHData data, SenderThread sender, ICallback callback)
		{
			m_data = data;
			m_sender = sender;
			m_callback = callback;
		}

		public void VisitInit(NetPacketInit p)
		{
			throw new NotImplementedException();
		}

		public void VisitInitAck(NetPacketInitAck p)
		{
			m_data.MyPlayerId =
				Convert.ToUInt32(p.Properties[NetPacket.PropertyType.PlayerId]);
			m_callback.InitDone();
		}

		public void VisitGameListNew(NetPacketGameListNew p)
		{
			// Add game to list.
			m_data.GameList.AddGameInfo(new GameInfo(
				Convert.ToUInt32(p.Properties[NetPacket.PropertyType.GameId]),
				p.Properties[NetPacket.PropertyType.GameName],
				(GameInfo.Mode)Convert.ToInt32(p.Properties[NetPacket.PropertyType.GameMode]),
				p.ListProperties[NetPacket.ListPropertyType.PropPlayerSlots].
					ConvertAll<uint>(Convert.ToUInt32)));
		}

		public void VisitGameListUpdate(NetPacketGameListUpdate p)
		{
			GameInfo.Mode mode =
				(GameInfo.Mode)Convert.ToInt32(p.Properties[NetPacket.PropertyType.GameMode]);
			uint id = Convert.ToUInt32(p.Properties[NetPacket.PropertyType.GameId]);
			if (mode == GameInfo.Mode.Closed) // Remove game if it is has been closed.
				m_data.GameList.RemoveGameInfo(id);
			else
				m_data.GameList.GetGameInfo(id).CurrentMode = mode;
		}

		public void VisitRetrievePlayerInfo(NetPacketRetrievePlayerInfo p)
		{
			throw new NotImplementedException();
		}

		public void VisitPlayerInfo(NetPacketPlayerInfo p)
		{
			// Add player to list.
			m_data.PlayerList.AddPlayerInfo(new PlayerInfo(
				Convert.ToUInt32(p.Properties[NetPacket.PropertyType.PlayerId]),
				p.Properties[NetPacket.PropertyType.PlayerName]));
		}

		public void VisitJoinGame(NetPacketJoinGame p)
		{
			throw new NotImplementedException();
		}

		public void VisitJoinGameAck(NetPacketJoinGameAck p)
		{
			m_data.MyGameId =
				Convert.ToUInt32(p.Properties[NetPacket.PropertyType.GameId]);
			m_callback.JoinedGame(m_data.GameList.GetGameInfo(m_data.MyGameId).Name);
		}

		public void VisitStartEvent(NetPacketStartEvent p)
		{
			// Request player names for player ids.
			List<uint> playerSlots = m_data.GameList.GetGameInfo(m_data.MyGameId).PlayerSlots;
			foreach (uint id in playerSlots)
			{
				if (!m_data.PlayerList.HasPlayer(id))
				{
					NetPacketRetrievePlayerInfo request = new NetPacketRetrievePlayerInfo();
					request.Properties.Add(NetPacket.PropertyType.PlayerId, Convert.ToString(id));
					m_sender.Send(request);
				}
			}
			// Acknowledge start event.
			NetPacketStartEventAck ack = new NetPacketStartEventAck();
			m_sender.Send(ack);
		}

		public void VisitStartEventAck(NetPacketStartEventAck p)
		{
			throw new NotImplementedException();
		}

		public void VisitGameStart(NetPacketGameStart p)
		{
			// Generate player list.
			List<string> players = new List<string>();

			List<uint> slots = p.ListProperties[NetPacket.ListPropertyType.PropPlayerSlots].
				ConvertAll<uint>(Convert.ToUInt32);
			foreach (uint i in slots)
			{
				if (m_data.PlayerList.HasPlayer(i))
					players.Add(m_data.PlayerList.GetPlayerInfo(i).Name);
				else if (i == m_data.MyPlayerId)
					players.Add(m_data.MyName);
				else
					players.Add(Convert.ToString(i));
			}

			m_callback.GameStarted(players);
		}

		public void VisitHandStart(NetPacketHandStart p)
		{
			m_callback.HandStarted(
				Convert.ToInt32(p.Properties[NetPacket.PropertyType.FirstCard]),
				Convert.ToInt32(p.Properties[NetPacket.PropertyType.SecondCard]));
		}

		public void VisitPlayersTurn(NetPacketPlayersTurn p)
		{
			// TODO
		}

		public void VisitPlayersAction(NetPacketPlayersAction p)
		{
			throw new NotImplementedException();
		}

		public void VisitPlayersActionDone(NetPacketPlayersActionDone p)
		{
			// TODO
		}

		public void VisitPlayersActionRejected(NetPacketPlayersActionRejected p)
		{
			// TODO
		}

		private PokerTHData m_data;
		private SenderThread m_sender;
		private ICallback m_callback;
	}
}
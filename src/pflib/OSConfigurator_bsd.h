/* 

                          Firewall Builder

                 Copyright (C) 2002,2009 NetCitadel, LLC

  Author:  Vadim Kurland     vadim@vk.crocodile.org

  $Id$

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef _OSCONFIGURATOR_BSD_HH
#define _OSCONFIGURATOR_BSD_HH

#include "config.h"

#include "fwcompiler/OSConfigurator.h"

#include <set>

#include <QMap>
#include <QStringList>


class Configlet;

namespace libfwbuilder
{
    class Firewall;
    class Interface;
    class StateSyncClusterGroup;
}

namespace fwcompiler
{

    class OSConfigurator_bsd : public OSConfigurator
    {
        
protected:
        QMap<QString, QString> update_address_lines;
        QStringList cloned_interfaces;
        QStringList interface_configuration_lines;

	std::set<const libfwbuilder::Address*> virtual_addresses;

        virtual void setKernelVariable(libfwbuilder::Firewall *fw,
                                       const std::string &var_name,
                                       Configlet *configlet);

        // functions that generate interface address configuration
        virtual void listAllInterfacesConfigLine(QStringList intf_names,
                                                    bool ipv6);

        virtual void updateAddressesOfInterface(
            libfwbuilder::Interface *iface,
            std::list<std::pair<libfwbuilder::InetAddr,libfwbuilder::InetAddr> >
                all_addresses);

        // functions that generate VLAN configuration
        virtual void listAllVlansConfgLine(QStringList vlan_names);

        virtual void updateVlansOfInterface(
            libfwbuilder::Interface *iface, QStringList vlan_names);

        // functions that generate bridge configuration
        virtual void listAllBridgeConfgLine(QStringList vlan_names);

        virtual void updateBridgeOfInterface(
            libfwbuilder::Interface *iface, QStringList vlan_names);

        // functions that generate CARP interface configuration
        virtual void listAllCARPConfgLine(QStringList carp_names);
        virtual void updateCARPInterface(libfwbuilder::Interface *iface,
                                            libfwbuilder::FWObject *failover_group);
        virtual void updateCARPInterfaceInternal(
            libfwbuilder::Interface *iface,
            libfwbuilder::FWObject *failover_group,
            Configlet *configlet);

        // functions that generate pfsync interface configuration
        virtual void listAllPfsyncConfgLine(bool have_pfsync);
        virtual void updatePfsyncInterface(
            libfwbuilder::Interface *iface,
            libfwbuilder::StateSyncClusterGroup *sync_group);

        virtual QString printAllInterfaceConfigurationLines();

public:

	virtual ~OSConfigurator_bsd() {};
        OSConfigurator_bsd(libfwbuilder::FWObjectDatabase *_db,
                           libfwbuilder::Firewall *fw,
                           bool ipv6_policy) : 
        OSConfigurator(_db, fw, ipv6_policy) {}

        virtual int prolog();

	virtual std::string myPlatformName();
	virtual void addVirtualAddressForNAT(const libfwbuilder::Address *addr);
	virtual void addVirtualAddressForNAT(const libfwbuilder::Network   *nw);

        virtual std::string printFunctions();
	virtual std::string printKernelVarsCommands();
        virtual std::string configureInterfaces();

        std::string getInterfaceVarName(libfwbuilder::FWObject *iface);

        virtual void processFirewallOptions() {}
    };
};

#endif

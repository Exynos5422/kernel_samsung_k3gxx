/*
 * Copyright (C) 2012 - ARM Ltd
 * Author: Marc Zyngier <marc.zyngier@arm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __ARM_KVM_PSCI_H__
#define __ARM_KVM_PSCI_H__

<<<<<<< HEAD
bool kvm_psci_call(struct kvm_vcpu *vcpu);
=======
#define KVM_ARM_PSCI_0_1	1
#define KVM_ARM_PSCI_0_2	2

int kvm_psci_version(struct kvm_vcpu *vcpu);
int kvm_psci_call(struct kvm_vcpu *vcpu);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83

#endif /* __ARM_KVM_PSCI_H__ */

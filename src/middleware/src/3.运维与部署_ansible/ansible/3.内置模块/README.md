# [Ansible.Builtin](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#ansible-builtin)

Collection version 2.12.6.post0

- [Description](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#description)
- [Communication](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#communication)
- [Plugin Index](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#plugin-index)

## [Description](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#id1)

这些都是ansible-core中包含的所有模块和插件。

**Author:**

- Ansible, Inc.

[Issue Tracker](https://github.com/ansible/ansible/issues)[Repository (Sources)](https://github.com/ansible/ansible)



## [Communication](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#id2)

- Matrix room `#users:ansible.im`: [General usage and support questions](https://matrix.to/#/#users:ansible.im).
- IRC channel `#ansible` (Libera network): [General usage and support questions](https://web.libera.chat/?channel=#ansible).
- Mailing list: [Ansible Project List](https://groups.google.com/g/ansible-project). ([Subscribe](mailto:ansible-project+subscribe@googlegroups.com?subject=subscribe))

## [Plugin Index](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#id3)

这些是ansible.builtin集合中的插件:

### Modules[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#modules)

- [add_host module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/add_host_module.html#ansible-collections-ansible-builtin-add-host-module) – Add a host (and alternatively a group) to the ansible-playbook in-memory inventory
- [apt module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/apt_module.html#ansible-collections-ansible-builtin-apt-module) – Manages apt-packages
- [apt_key module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/apt_key_module.html#ansible-collections-ansible-builtin-apt-key-module) – Add or remove an apt key
- [apt_repository module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/apt_repository_module.html#ansible-collections-ansible-builtin-apt-repository-module) – Add and remove APT repositories
- [assemble module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/assemble_module.html#ansible-collections-ansible-builtin-assemble-module) – Assemble configuration files from fragments
- [assert module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/assert_module.html#ansible-collections-ansible-builtin-assert-module) – Asserts given expressions are true
- [async_status module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/async_status_module.html#ansible-collections-ansible-builtin-async-status-module) – Obtain status of asynchronous task
- [blockinfile module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/blockinfile_module.html#ansible-collections-ansible-builtin-blockinfile-module) – Insert/update/remove a text block surrounded by marker lines
- [command module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/command_module.html#ansible-collections-ansible-builtin-command-module) – Execute commands on targets
- [copy module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/copy_module.html#ansible-collections-ansible-builtin-copy-module) – Copy files to remote locations
- [cron module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/cron_module.html#ansible-collections-ansible-builtin-cron-module) – Manage cron.d and crontab entries
- [debconf module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/debconf_module.html#ansible-collections-ansible-builtin-debconf-module) – Configure a .deb package
- [debug module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/debug_module.html#ansible-collections-ansible-builtin-debug-module) – Print statements during execution
- [dnf module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/dnf_module.html#ansible-collections-ansible-builtin-dnf-module) – Manages packages with the *dnf* package manager
- [dpkg_selections module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/dpkg_selections_module.html#ansible-collections-ansible-builtin-dpkg-selections-module) – Dpkg package selection selections
- [expect module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/expect_module.html#ansible-collections-ansible-builtin-expect-module) – Executes a command and responds to prompts
- [fail module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/fail_module.html#ansible-collections-ansible-builtin-fail-module) – Fail with custom message
- [fetch module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/fetch_module.html#ansible-collections-ansible-builtin-fetch-module) – Fetch files from remote nodes
- [file module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/file_module.html#ansible-collections-ansible-builtin-file-module) – Manage files and file properties
- [find module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/find_module.html#ansible-collections-ansible-builtin-find-module) – Return a list of files based on specific criteria
- [gather_facts module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/gather_facts_module.html#ansible-collections-ansible-builtin-gather-facts-module) – Gathers facts about remote hosts
- [get_url module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/get_url_module.html#ansible-collections-ansible-builtin-get-url-module) – Downloads files from HTTP, HTTPS, or FTP to node
- [getent module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/getent_module.html#ansible-collections-ansible-builtin-getent-module) – A wrapper to the unix getent utility
- [git module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/git_module.html#ansible-collections-ansible-builtin-git-module) – Deploy software (or files) from git checkouts
- [group module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/group_module.html#ansible-collections-ansible-builtin-group-module) – Add or remove groups
- [group_by module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/group_by_module.html#ansible-collections-ansible-builtin-group-by-module) – Create Ansible groups based on facts
- [hostname module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/hostname_module.html#ansible-collections-ansible-builtin-hostname-module) – Manage hostname
- [import_playbook module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/import_playbook_module.html#ansible-collections-ansible-builtin-import-playbook-module) – Import a playbook
- [import_role module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/import_role_module.html#ansible-collections-ansible-builtin-import-role-module) – Import a role into a play
- [import_tasks module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/import_tasks_module.html#ansible-collections-ansible-builtin-import-tasks-module) – Import a task list
- [include module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/include_module.html#ansible-collections-ansible-builtin-include-module) – Include a play or task list
- [include_role module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/include_role_module.html#ansible-collections-ansible-builtin-include-role-module) – Load and execute a role
- [include_tasks module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/include_tasks_module.html#ansible-collections-ansible-builtin-include-tasks-module) – Dynamically include a task list
- [include_vars module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/include_vars_module.html#ansible-collections-ansible-builtin-include-vars-module) – Load variables from files, dynamically within a task
- [iptables module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/iptables_module.html#ansible-collections-ansible-builtin-iptables-module) – Modify iptables rules
- [known_hosts module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/known_hosts_module.html#ansible-collections-ansible-builtin-known-hosts-module) – Add or remove a host from the `known_hosts` file
- [lineinfile module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/lineinfile_module.html#ansible-collections-ansible-builtin-lineinfile-module) – Manage lines in text files
- [meta module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/meta_module.html#ansible-collections-ansible-builtin-meta-module) – Execute Ansible ‘actions’
- [package module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/package_module.html#ansible-collections-ansible-builtin-package-module) – Generic OS package manager
- [package_facts module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/package_facts_module.html#ansible-collections-ansible-builtin-package-facts-module) – Package information as facts
- [pause module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/pause_module.html#ansible-collections-ansible-builtin-pause-module) – Pause playbook execution
- [ping module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/ping_module.html#ansible-collections-ansible-builtin-ping-module) – Try to connect to host, verify a usable python and return `pong` on success
- [pip module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/pip_module.html#ansible-collections-ansible-builtin-pip-module) – Manages Python library dependencies
- [raw module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/raw_module.html#ansible-collections-ansible-builtin-raw-module) – Executes a low-down and dirty command
- [reboot module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/reboot_module.html#ansible-collections-ansible-builtin-reboot-module) – Reboot a machine
- [replace module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/replace_module.html#ansible-collections-ansible-builtin-replace-module) – Replace all instances of a particular string in a file using a back-referenced regular expression
- [rpm_key module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/rpm_key_module.html#ansible-collections-ansible-builtin-rpm-key-module) – Adds or removes a gpg key from the rpm db
- [script module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/script_module.html#ansible-collections-ansible-builtin-script-module) – Runs a local script on a remote node after transferring it
- [service module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/service_module.html#ansible-collections-ansible-builtin-service-module) – Manage services
- [service_facts module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/service_facts_module.html#ansible-collections-ansible-builtin-service-facts-module) – Return service state information as fact data
- [set_fact module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/set_fact_module.html#ansible-collections-ansible-builtin-set-fact-module) – Set host variable(s) and fact(s).
- [set_stats module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/set_stats_module.html#ansible-collections-ansible-builtin-set-stats-module) – Define and display stats for the current ansible run
- [setup module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/setup_module.html#ansible-collections-ansible-builtin-setup-module) – Gathers facts about remote hosts
- [shell module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/shell_module.html#ansible-collections-ansible-builtin-shell-module) – Execute shell commands on targets
- [slurp module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/slurp_module.html#ansible-collections-ansible-builtin-slurp-module) – Slurps a file from remote nodes
- [stat module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/stat_module.html#ansible-collections-ansible-builtin-stat-module) – Retrieve file or file system status
- [subversion module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/subversion_module.html#ansible-collections-ansible-builtin-subversion-module) – Deploys a subversion repository
- [systemd module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/systemd_module.html#ansible-collections-ansible-builtin-systemd-module) – Manage systemd units
- [sysvinit module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/sysvinit_module.html#ansible-collections-ansible-builtin-sysvinit-module) – Manage SysV services.
- [tempfile module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/tempfile_module.html#ansible-collections-ansible-builtin-tempfile-module) – Creates temporary files and directories
- [template module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/template_module.html#ansible-collections-ansible-builtin-template-module) – Template a file out to a target host
- [unarchive module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/unarchive_module.html#ansible-collections-ansible-builtin-unarchive-module) – Unpacks an archive after (optionally) copying it from the local machine
- [uri module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/uri_module.html#ansible-collections-ansible-builtin-uri-module) – Interacts with webservices
- [user module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/user_module.html#ansible-collections-ansible-builtin-user-module) – Manage user accounts
- [validate_argument_spec module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/validate_argument_spec_module.html#ansible-collections-ansible-builtin-validate-argument-spec-module) – Validate role argument specs.
- [wait_for module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/wait_for_module.html#ansible-collections-ansible-builtin-wait-for-module) – Waits for a condition before continuing
- [wait_for_connection module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/wait_for_connection_module.html#ansible-collections-ansible-builtin-wait-for-connection-module) – Waits until remote system is reachable/usable
- [yum module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/yum_module.html#ansible-collections-ansible-builtin-yum-module) – Manages packages with the *yum* package manager
- [yum_repository module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/yum_repository_module.html#ansible-collections-ansible-builtin-yum-repository-module) – Add or remove YUM repositories

### Become Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#become-plugins)

- [runas become](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/runas_become.html#ansible-collections-ansible-builtin-runas-become) – Run As user
- [su become](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/su_become.html#ansible-collections-ansible-builtin-su-become) – Substitute User
- [sudo become](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/sudo_become.html#ansible-collections-ansible-builtin-sudo-become) – Substitute User DO

### Cache Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#cache-plugins)

- [jsonfile cache](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/jsonfile_cache.html#ansible-collections-ansible-builtin-jsonfile-cache) – JSON formatted files.
- [memory cache](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/memory_cache.html#ansible-collections-ansible-builtin-memory-cache) – RAM backed, non persistent

### Callback Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#callback-plugins)

- [default callback](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/default_callback.html#ansible-collections-ansible-builtin-default-callback) – default Ansible screen output
- [junit callback](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/junit_callback.html#ansible-collections-ansible-builtin-junit-callback) – write playbook output to a JUnit file.
- [minimal callback](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/minimal_callback.html#ansible-collections-ansible-builtin-minimal-callback) – minimal Ansible screen output
- [oneline callback](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/oneline_callback.html#ansible-collections-ansible-builtin-oneline-callback) – oneline Ansible screen output
- [tree callback](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/tree_callback.html#ansible-collections-ansible-builtin-tree-callback) – Save host events to files

### Connection Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#connection-plugins)

- [local connection](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/local_connection.html#ansible-collections-ansible-builtin-local-connection) – execute on controller
- [paramiko_ssh connection](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/paramiko_ssh_connection.html#ansible-collections-ansible-builtin-paramiko-ssh-connection) – Run tasks via python ssh (paramiko)
- [psrp connection](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/psrp_connection.html#ansible-collections-ansible-builtin-psrp-connection) – Run tasks over Microsoft PowerShell Remoting Protocol
- [ssh connection](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/ssh_connection.html#ansible-collections-ansible-builtin-ssh-connection) – connect via SSH client binary
- [winrm connection](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/winrm_connection.html#ansible-collections-ansible-builtin-winrm-connection) – Run tasks over Microsoft’s WinRM

### Inventory Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#inventory-plugins)

- [advanced_host_list inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/advanced_host_list_inventory.html#ansible-collections-ansible-builtin-advanced-host-list-inventory) – Parses a ‘host list’ with ranges
- [auto inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/auto_inventory.html#ansible-collections-ansible-builtin-auto-inventory) – Loads and executes an inventory plugin specified in a YAML config
- [constructed inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/constructed_inventory.html#ansible-collections-ansible-builtin-constructed-inventory) – Uses Jinja2 to construct vars and groups based on existing inventory.
- [generator inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/generator_inventory.html#ansible-collections-ansible-builtin-generator-inventory) – Uses Jinja2 to construct hosts and groups from patterns
- [host_list inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/host_list_inventory.html#ansible-collections-ansible-builtin-host-list-inventory) – Parses a ‘host list’ string
- [ini inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/ini_inventory.html#ansible-collections-ansible-builtin-ini-inventory) – Uses an Ansible INI file as inventory source.
- [script inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/script_inventory.html#ansible-collections-ansible-builtin-script-inventory) – Executes an inventory script that returns JSON
- [toml inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/toml_inventory.html#ansible-collections-ansible-builtin-toml-inventory) – Uses a specific TOML file as an inventory source.
- [yaml inventory](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/yaml_inventory.html#ansible-collections-ansible-builtin-yaml-inventory) – Uses a specific YAML file as an inventory source.

### Lookup Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#lookup-plugins)

- [config lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/config_lookup.html#ansible-collections-ansible-builtin-config-lookup) – Lookup current Ansible configuration values
- [csvfile lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/csvfile_lookup.html#ansible-collections-ansible-builtin-csvfile-lookup) – read data from a TSV or CSV file
- [dict lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/dict_lookup.html#ansible-collections-ansible-builtin-dict-lookup) – returns key/value pair items from dictionaries
- [env lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/env_lookup.html#ansible-collections-ansible-builtin-env-lookup) – Read the value of environment variables
- [file lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/file_lookup.html#ansible-collections-ansible-builtin-file-lookup) – read file contents
- [fileglob lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/fileglob_lookup.html#ansible-collections-ansible-builtin-fileglob-lookup) – list files matching a pattern
- [first_found lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/first_found_lookup.html#ansible-collections-ansible-builtin-first-found-lookup) – return first file found from list
- [indexed_items lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/indexed_items_lookup.html#ansible-collections-ansible-builtin-indexed-items-lookup) – rewrites lists to return ‘indexed items’
- [ini lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/ini_lookup.html#ansible-collections-ansible-builtin-ini-lookup) – read data from a ini file
- [inventory_hostnames lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/inventory_hostnames_lookup.html#ansible-collections-ansible-builtin-inventory-hostnames-lookup) – list of inventory hosts matching a host pattern
- [items lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/items_lookup.html#ansible-collections-ansible-builtin-items-lookup) – list of items
- [lines lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/lines_lookup.html#ansible-collections-ansible-builtin-lines-lookup) – read lines from command
- [list lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/list_lookup.html#ansible-collections-ansible-builtin-list-lookup) – simply returns what it is given.
- [nested lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/nested_lookup.html#ansible-collections-ansible-builtin-nested-lookup) – composes a list with nested elements of other lists
- [password lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/password_lookup.html#ansible-collections-ansible-builtin-password-lookup) – retrieve or generate a random password, stored in a file
- [pipe lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/pipe_lookup.html#ansible-collections-ansible-builtin-pipe-lookup) – read output from a command
- [random_choice lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/random_choice_lookup.html#ansible-collections-ansible-builtin-random-choice-lookup) – return random element from list
- [sequence lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/sequence_lookup.html#ansible-collections-ansible-builtin-sequence-lookup) – generate a list based on a number sequence
- [subelements lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/subelements_lookup.html#ansible-collections-ansible-builtin-subelements-lookup) – traverse nested key from a list of dictionaries
- [template lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/template_lookup.html#ansible-collections-ansible-builtin-template-lookup) – retrieve contents of file after templating with Jinja2
- [together lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/together_lookup.html#ansible-collections-ansible-builtin-together-lookup) – merges lists into synchronized list
- [unvault lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/unvault_lookup.html#ansible-collections-ansible-builtin-unvault-lookup) – read vaulted file(s) contents
- [url lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/url_lookup.html#ansible-collections-ansible-builtin-url-lookup) – return contents from URL
- [varnames lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/varnames_lookup.html#ansible-collections-ansible-builtin-varnames-lookup) – Lookup matching variable names
- [vars lookup](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/vars_lookup.html#ansible-collections-ansible-builtin-vars-lookup) – Lookup templated value of variables

### Shell Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#shell-plugins)

- [cmd shell](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/cmd_shell.html#ansible-collections-ansible-builtin-cmd-shell) – Windows Command Prompt
- [powershell shell](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/powershell_shell.html#ansible-collections-ansible-builtin-powershell-shell) – Windows PowerShell
- [sh shell](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/sh_shell.html#ansible-collections-ansible-builtin-sh-shell) – POSIX shell (/bin/sh)

### Strategy Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#strategy-plugins)

- [debug strategy](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/debug_strategy.html#ansible-collections-ansible-builtin-debug-strategy) – Executes tasks in interactive debug session.
- [free strategy](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/free_strategy.html#ansible-collections-ansible-builtin-free-strategy) – Executes tasks without waiting for all hosts
- [host_pinned strategy](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/host_pinned_strategy.html#ansible-collections-ansible-builtin-host-pinned-strategy) – Executes tasks on each host without interruption
- [linear strategy](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/linear_strategy.html#ansible-collections-ansible-builtin-linear-strategy) – Executes tasks in a linear fashion

### Vars Plugins[](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/index.html#vars-plugins)

- [host_group_vars vars](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/host_group_vars_vars.html#ansible-collections-ansible-builtin-host-group-vars-vars) – In charge of loading group_vars and host_vars







List of [collections](https://docs.ansible.com/ansible/latest/collections/index.html#list-of-collections) with docs hosted here.

[ Previous](https://docs.ansible.com/ansible/latest/collections/ansible/index.html)[Next ](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/runas_become.html)
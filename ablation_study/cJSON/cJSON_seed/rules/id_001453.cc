#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *strref = NULL;
    cJSON *true_item = NULL;
    cJSON *num_item = NULL;
    char prebuf[256];
    cJSON_bool printed_ok = 0;
    cJSON_bool is_true = 0;
    cJSON_bool add_ok_str = 0;
    cJSON_bool add_ok_true = 0;
    cJSON_bool add_ok_num = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    strref = cJSON_CreateStringReference("example");
    add_ok_str = cJSON_AddItemToObject(root, "label", strref);
    true_item = cJSON_CreateTrue();
    add_ok_true = cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    memset(prebuf, 0, sizeof(prebuf));
    printed_ok = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 1);
    is_true = cJSON_IsTrue(true_item);
    add_ok_num = (num_item != NULL);
    (void)printed_ok;
    (void)is_true;
    (void)add_ok_str;
    (void)add_ok_true;
    (void)add_ok_num;
    (void)num_item;
    (void)strref;
    (void)true_item;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *orig = (cJSON *)0;
    cJSON *repl_str = (cJSON *)0;
    cJSON *repl_false = (cJSON *)0;
    cJSON *got = (cJSON *)0;
    cJSON *mid = (cJSON *)0;
    cJSON *final_item = (cJSON *)0;
    const char *str_after = (const char *)0;
    cJSON_bool add_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("original");
    repl_str = cJSON_CreateString("replaced");
    repl_false = cJSON_CreateFalse();

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "node", orig);

    // step 4: Operate
    got = cJSON_GetObjectItem(root, "node");
    cJSON_ReplaceItemViaPointer(root, got, repl_str);
    mid = cJSON_GetObjectItem(root, "node");
    str_after = cJSON_GetStringValue(mid);
    cJSON_ReplaceItemViaPointer(root, mid, repl_false);
    final_item = cJSON_GetObjectItem(root, "node");

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(got != (cJSON *)0) + (int)(mid != (cJSON *)0) + (int)(final_item != (cJSON *)0) + (int)(str_after != (const char *)0) + (int)(cJSON_IsFalse(final_item) != 0) + (int)(add_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)orig;
    (void)repl_str;
    (void)repl_false;
    (void)got;
    (void)mid;
    (void)final_item;
    (void)str_after;
    (void)add_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_a = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_a);
    void *mem = cJSON_malloc(16);
    memset(mem, 0, 16);
    char *keybuf = (char*)mem;
    keybuf[0] = 't';
    keybuf[1] = 'e';
    keybuf[2] = 'm';
    keybuf[3] = 'p';
    keybuf[4] = '\0';

    // step 2: Configure
    cJSON *strref = cJSON_CreateStringReference(keybuf);
    cJSON_AddItemToObject(root, keybuf, strref);
    cJSON *dup = cJSON_Duplicate(null_a, 1);
    cJSON_AddItemToObject(root, "empty_copy", dup);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateNull();
    cJSON_ReplaceItemViaPointer(root, dup, replacement);
    cJSON *root_copy = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    cJSON_free(mem);
    return 66; // API sequence test completed successfully
}
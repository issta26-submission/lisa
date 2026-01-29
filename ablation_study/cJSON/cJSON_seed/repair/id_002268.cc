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
//<ID> 2268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *original = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "original", original);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(original, "name", name);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(original, "pi", pi);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(original, 1);
    cJSON_AddItemToObject(root, "copy", dup);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObject(root, "version", ver_item);

    // step 3: Operate and Validate
    cJSON *existing_pi = cJSON_GetObjectItem(original, "pi");
    cJSON *new_pi = cJSON_CreateNumber(2.71828);
    cJSON_ReplaceItemViaPointer(original, existing_pi, new_pi);
    cJSON *dup_new_pi = cJSON_Duplicate(new_pi, 0);
    cJSON_AddItemToObject(root, "replaced_pi", dup_new_pi);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = ver[0];
    buffer[1] = (char)(original->type & 0xFF);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
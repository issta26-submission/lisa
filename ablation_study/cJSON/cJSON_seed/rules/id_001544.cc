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
//<ID> 1544
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *list = NULL;
    cJSON *child = NULL;
    cJSON *arr_ref = NULL;
    cJSON *greeting = NULL;
    char *printed = NULL;
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "enabled");
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    list = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(list, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(list, cJSON_CreateString("beta"));
    child = cJSON_CreateArray();
    cJSON_AddItemToArray(child, cJSON_CreateString("one"));
    cJSON_AddItemToArray(child, cJSON_CreateString("two"));
    arr_ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToObject(root, "ref_items", arr_ref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    scratch[0] = printed[0];
    scratch[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(child);
    // API sequence test completed successfully
    return 66;
}
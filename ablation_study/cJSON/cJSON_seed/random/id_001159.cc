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
//<ID> 1159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *raw_added = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create string items and add them to the array
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 4: Configure (continued) - add a raw JSON fragment as a member of root
    raw_added = cJSON_AddRawToObject(root, "meta", "{\"version\":2,\"notes\":\"raw-data\"}");

    // step 5: Operate / Validate - inspect types and array contents, serialize for round-trip
    fetched0 = cJSON_GetArrayItem(arr, 0);
    validation_score = (int)cJSON_IsString(s2) + (int)cJSON_IsString(fetched0) + (int)(raw_added != (cJSON *)0);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
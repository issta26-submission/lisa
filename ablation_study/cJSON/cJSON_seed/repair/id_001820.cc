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
//<ID> 1820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 42.5);
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(ver));

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *it1 = cJSON_CreateObject();
    cJSON_AddItemToObject(it1, "id", cJSON_CreateNumber(1.0));
    cJSON_AddItemToObject(it1, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, it1);
    cJSON *it2 = cJSON_Duplicate(it1, 1);
    cJSON_SetValuestring(cJSON_GetObjectItem(it2, "name"), "beta");
    cJSON_AddItemToArray(items, it2);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *first_name = cJSON_GetStringValue(cJSON_GetObjectItem(it1, "name"));
    double thr = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "config"), "threshold"));
    double id0 = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetArrayItem(items, 0), "id"));
    buffer[0] = first_name ? first_name[0] : 'X';
    buffer[1] = ver ? ver[0] : 'V';
    buffer[2] = (char)('0' + (((int)thr) % 10));
    buffer[3] = (char)('0' + (((int)id0) % 10));
    (void)buffer; (void)printed; (void)first_name; (void)ver; (void)thr; (void)id0;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
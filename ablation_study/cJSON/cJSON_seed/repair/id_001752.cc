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
//<ID> 1752
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *name = cJSON_CreateString("fuzzcase");
    cJSON_AddItemToObject(data, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON *mirror = cJSON_CreateObject();
    cJSON *mdata = cJSON_CreateObject();
    cJSON_AddItemToObject(mirror, "data", mdata);
    cJSON_AddItemToObject(mdata, "name", cJSON_CreateString("fuzzcase"));
    cJSON *mvalues = cJSON_CreateArray();
    cJSON_AddItemToObject(mdata, "values", mvalues);
    cJSON_AddItemToArray(mvalues, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(mvalues, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(mvalues, cJSON_CreateNumber(30.0));

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_bool equal = cJSON_Compare(root, mirror, 1);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = equal ? '1' : '0';
    buffer[2] = (char)('0' + (int)cJSON_GetArraySize(values));
    (void)buffer;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(mirror);
    return 66; // API sequence test completed successfully
}
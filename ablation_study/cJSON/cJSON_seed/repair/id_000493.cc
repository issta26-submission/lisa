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
//<ID> 493
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"initial\":true}");
    cJSON *array_main = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(42.0);
    cJSON *n1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(array_main, n0);
    cJSON_AddItemToArray(array_main, n1);
    cJSON_AddItemToObject(root, "vals", array_main);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddItemToObject(meta, "count", cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(meta, "name", cJSON_CreateString("example"));

    // step 3: Operate
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToArray(extras, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(extras, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(meta, "extras", extras);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON *embedded = cJSON_Parse("{\"embedded\":123}");
    cJSON_AddItemToObject(root, "embedded_parsed", embedded);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.5));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(4.5));

    // step 2: Configure
    cJSON_AddStringToObject(config, "name", "example");
    cJSON *meta = cJSON_AddObjectToObject(config, "meta");
    cJSON_AddNumberToObject(meta, "scale", 1.5);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(values);
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double computed = first_val * scale + (double)size;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
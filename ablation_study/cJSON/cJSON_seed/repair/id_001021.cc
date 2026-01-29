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
//<ID> 1021
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON *data = cJSON_AddArrayToObject(root, "data");
    cJSON_AddItemToArray(data, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(20.0));

    // step 2: Configure
    char *buf = (char *)cJSON_malloc(64);
    memset(buf, 0, 64);
    buf[0] = 'i';
    buf[1] = 'd';
    buf[2] = ':';
    buf[3] = '1';
    buf[4] = '\0';
    cJSON_AddStringToObject(root, "id", buf);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetObjectItem(root, "data");
    cJSON_bool is_array = cJSON_IsArray(got);
    cJSON_AddNumberToObject(root, "data_is_array", (double)is_array);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buf);
    // API sequence test completed successfully
    return 66;
}
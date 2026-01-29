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
//<ID> 161
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
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(data, "nullable", nullable);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(data, "answer", num);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(data, "greeting", greeting);
    char *buf = (char *)cJSON_malloc(10);
    buf[0] = 'a'; buf[1] = 'l'; buf[2] = 'l'; buf[3] = 'o'; buf[4] = 'c';
    buf[5] = 'a'; buf[6] = 't'; buf[7] = 'e'; buf[8] = 'd'; buf[9] = '\0';
    cJSON *from_malloc = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "from_malloc", from_malloc);
    cJSON_free(buf);

    // step 3: Operate and Validate
    cJSON *dup_data = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "data_copy", dup_data);
    cJSON *replacement = cJSON_CreateNumber(100.0);
    cJSON_ReplaceItemViaPointer(data, nullable, replacement);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}
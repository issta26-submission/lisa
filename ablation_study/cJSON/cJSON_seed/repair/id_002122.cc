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
//<ID> 2122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_bool ok1 = cJSON_AddItemToArray(values, n1);
    (void)ok1;
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_bool ok2 = cJSON_AddItemToArray(values, n2);
    (void)ok2;
    cJSON_AddNumberToObject(root, "average", 15.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(meta, "greeting", greet);
    cJSON *status = cJSON_AddStringToObject(meta, "status", "ok");
    (void)status;
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *avg_item = cJSON_GetObjectItemCaseSensitive(root, "average");
    double avg = cJSON_GetNumberValue(avg_item);
    cJSON *n3 = cJSON_CreateNumber(avg + 5.0);
    cJSON_bool ok3 = cJSON_AddItemToArray(values, n3);
    (void)ok3;

    // step 3: Operate and Validate
    cJSON *greet_item = cJSON_GetObjectItemCaseSensitive(meta, "greeting");
    const char *greet_str = cJSON_GetStringValue(greet_item);
    const int buflen = 32;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = greet_str ? greet_str[0] : '\0';
    int ai = (int)avg;
    buffer[1] = (char)('0' + (ai % 10));
    buffer[2] = '\0';
    cJSON_AddStringToObject(root, "summary", buffer);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
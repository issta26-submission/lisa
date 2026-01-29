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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.72);
    cJSON *meta = cJSON_CreateObject();
    cJSON *score = cJSON_CreateNumber(99.5);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_meta = NULL;
    cJSON *retr_score = NULL;
    double score_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(meta, "score", score);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    retr_score = cJSON_GetObjectItem(retr_meta, "score");
    score_value = cJSON_GetNumberValue(retr_score);
    buffer[0] = (char)('0' + ((int)score_value % 10));
    buffer[1] = '-';
    buffer[2] = (char)('A' + (((int)score_value / 10) % 26));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}
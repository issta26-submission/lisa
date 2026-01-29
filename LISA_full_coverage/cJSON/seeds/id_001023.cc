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
//<ID> 1023
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"person\":{\"name\":\"Alice\",\"scores\":[10,20,30]},\"meta\":\"orig\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    int new_scores_arr[] = {4, 5, 6};
    cJSON *new_scores = cJSON_CreateIntArray(new_scores_arr, 3);
    cJSON_AddItemToObject(person, "new_scores", new_scores);
    cJSON *person_copy = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_copy", person_copy);

    // step 3: Operate
    cJSON *scores = cJSON_GetObjectItem(person, "scores");
    int scores_count = cJSON_GetArraySize(scores);
    cJSON *summary = cJSON_CreateObject();
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(summary, "name_copy", name_copy);
    cJSON *count_num = cJSON_CreateNumber((double)scores_count);
    cJSON_AddItemToObject(summary, "scores_count", count_num);
    char *summary_unformatted = cJSON_PrintUnformatted(summary);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(summary, print_buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(summary_unformatted);
    cJSON_free(print_buf);
    cJSON_Delete(summary);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
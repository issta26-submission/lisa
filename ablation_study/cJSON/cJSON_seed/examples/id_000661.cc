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
//<ID> 661
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    int numbers_static[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers_static, 4);

    // step 2: Configure
    cJSON_AddItemToObject(root, "enabled", flag_true);
    cJSON_AddItemToObject(root, "disabled", flag_false);
    cJSON_AddItemToObject(root, "sequence", int_array);
    cJSON *sequence_dup = cJSON_Duplicate(int_array, 1);
    cJSON_AddItemToObjectCS(root, "sequence_copy", sequence_dup);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snapshot_len = snapshot ? strlen(snapshot) : 0;
    char *buffer = (char *)cJSON_malloc(snapshot_len + 1);
    memset(buffer, 0, snapshot_len + 1);
    if (snapshot_len > 0) memcpy(buffer, snapshot, snapshot_len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_seq = cJSON_GetObjectItemCaseSensitive(parsed, "sequence");
    cJSON *parsed_seq_copy = cJSON_GetObjectItemCaseSensitive(parsed, "sequence_copy");
    int parsed_size = cJSON_GetArraySize(parsed_seq);
    int parsed_copy_size = cJSON_GetArraySize(parsed_seq_copy);
    (void)version;
    (void)parsed_size;
    (void)parsed_copy_size;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
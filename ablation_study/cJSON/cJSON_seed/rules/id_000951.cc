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
//<ID> 951
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *s3 = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *first_item = NULL;
    cJSON *dup_array = NULL;
    const char *s1_text = "alpha";
    const char *s2_text = "beta";
    const char *s3_text = "gamma";
    const char *first_str = NULL;
    char *printed_root = NULL;
    char *printed_dup = NULL;
    char *scratch = NULL;
    cJSON_bool insert_res = 0;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    array = cJSON_AddArrayToObject(root, "list");
    s1 = cJSON_CreateString(s1_text);
    s2 = cJSON_CreateString(s2_text);
    s3 = cJSON_CreateString(s3_text);
    cJSON_AddItemToArray(array, s1);
    insert_res = cJSON_InsertItemInArray(array, 1, s2);
    cJSON_AddItemToArray(array, s3);

    // step 3: Operate / Validate
    retrieved_array = cJSON_GetObjectItem(root, "list");
    first_item = cJSON_GetArrayItem(retrieved_array, 0);
    first_str = cJSON_GetStringValue(first_item);
    dup_array = cJSON_Duplicate(retrieved_array, 1);
    cmp_equal = cJSON_Compare(retrieved_array, dup_array, 1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_dup = cJSON_PrintUnformatted(dup_array);
    scratch = (char *)cJSON_malloc((size_t)16);
    memset(scratch, 0, (size_t)16);
    scratch[0] = first_str ? first_str[0] : '0';
    scratch[1] = (char)('0' + (int)cmp_equal);
    scratch[2] = (char)('0' + (int)insert_res);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_free(scratch);
    cJSON_Delete(dup_array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
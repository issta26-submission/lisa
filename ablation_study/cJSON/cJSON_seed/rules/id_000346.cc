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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("ALPHA");
    cJSON *ins = cJSON_CreateString("mid");
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    cJSON_bool is_s1_string = 0;
    cJSON_bool is_num_string = 0;
    cJSON_bool cmp_case_insensitive = 0;
    cJSON_bool cmp_case_sensitive = 0;
    cJSON *got0 = NULL;
    cJSON *got2 = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemReferenceToArray(arr, s2);
    cJSON_InsertItemInArray(arr, 1, ins);
    cJSON_AddStringToObject(root, "ver", version);

    // step 3: Operate / Validate
    got0 = cJSON_GetArrayItem(arr, 0);
    got2 = cJSON_GetArrayItem(arr, 2);
    is_num_string = cJSON_IsString(got0);
    is_s1_string = cJSON_IsString(got2);
    cmp_case_insensitive = cJSON_Compare(s1, s2, 0);
    cmp_case_sensitive = cJSON_Compare(s1, s2, 1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)is_num_string);
    buffer[1] = (char)('0' + (int)is_s1_string);
    buffer[2] = (char)('0' + (int)cmp_case_insensitive);
    buffer[3] = (char)('0' + (int)cmp_case_sensitive);
    buffer[4] = printed ? printed[0] : '\0';
    buffer[5] = version ? version[0] : '\0';
    (void)got0;
    (void)got2;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
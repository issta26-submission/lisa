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
//<ID> 942
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *s3 = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *greeting = "hello";
    const char *world = "world";
    const char *retrieved = NULL;
    int array_size = 0;
    size_t scratch_len = 64;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString(greeting);
    s2 = cJSON_CreateString(world);
    s3 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObjectCS(root, "greet_cs", s1);
    cJSON_AddItemToObject(root, "extra", cJSON_CreateString("beta"));

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    array_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "list"));
    retrieved = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greet_cs"));
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    scratch[0] = (retrieved != NULL && retrieved[0] != '\0') ? retrieved[0] : ' ';
    scratch[1] = (char)('0' + (array_size % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
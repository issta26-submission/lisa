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
//<ID> 901
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *flags = NULL;
    cJSON *flag0 = NULL;
    cJSON *flag1 = NULL;
    cJSON *flag2 = NULL;
    cJSON_bool is0 = 0;
    cJSON_bool is1 = 0;
    cJSON_bool is2 = 0;
    const char *orig = "{\"flags\":[true,false,true]}";
    char *json_copy = NULL;
    size_t len = 0;
    int true_count = 0;

    // step 2: Setup / Configure
    len = strlen(orig);
    json_copy = (char *)cJSON_malloc(len + 1);
    memset(json_copy, 0, len + 1);
    memcpy(json_copy, orig, len + 1);
    root = cJSON_Parse(json_copy);

    // step 3: Operate / Validate
    flags = cJSON_GetObjectItem(root, "flags");
    flag0 = cJSON_GetArrayItem(flags, 0);
    flag1 = cJSON_GetArrayItem(flags, 1);
    flag2 = cJSON_GetArrayItem(flags, 2);
    is0 = cJSON_IsTrue(flag0);
    is1 = cJSON_IsTrue(flag1);
    is2 = cJSON_IsTrue(flag2);
    true_count = (int)(is0 + is1 + is2);

    // step 4: Cleanup
    cJSON_free(json_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)flag0;
    (void)flag1;
    (void)flag2;
    (void)is0;
    (void)is1;
    (void)is2;
    (void)true_count;
    return 66;
}
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
//<ID> 640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(1024);
    memset(scratch, 0, 1024);
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");
    char *printed_unformatted = NULL;
    char *printed_formatted = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "s1", s1);
    cJSON_AddItemToObject(root, "s2", s2);
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON_AddStringToObject(obj, "inline", "inline_value");
    cJSON_AddRawToObject(root, "raw", "{\"nested\":true}");

    // step 3: Operate / Validate
    printed_unformatted = cJSON_PrintUnformatted(root);
    printed_len = printed_unformatted ? strlen(printed_unformatted) : 0;
    to_copy = printed_len < 1023 ? printed_len : 1023;
    memcpy(scratch, printed_unformatted ? printed_unformatted : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed_unformatted);
    printed_formatted = cJSON_Print(root);
    cJSON_free(printed_formatted);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
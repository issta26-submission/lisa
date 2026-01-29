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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateFalse();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *repl = cJSON_CreateTrue();
    cJSON *added_true = NULL;
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_flag = NULL;
    cJSON *retr_answer = NULL;
    double answer_val = 0.0;
    int added_nonnull = 0;
    int flag_true_int = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "answer", num);
    added_true = cJSON_AddTrueToObject(root, "always_true");
    added_nonnull = (added_true != NULL);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    cJSON_ReplaceItemInObject(parsed, "flag", repl);
    retr_flag = cJSON_GetObjectItem(parsed, "flag");
    retr_answer = cJSON_GetObjectItem(parsed, "answer");
    answer_val = cJSON_GetNumberValue(retr_answer);
    flag_true_int = (int)cJSON_IsTrue(retr_flag);
    buffer[0] = (char)('0' + ((int)answer_val % 10));
    buffer[1] = (char)('A' + (flag_true_int % 26));
    buffer[2] = (char)('a' + (added_nonnull % 26));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}